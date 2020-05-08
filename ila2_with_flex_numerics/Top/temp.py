"""
Very loosely based on https://github.com/OpenNMT/OpenNMT-py/blob/master/onmt/encoders/audio_encoder.py
"""
import tvm
from tvm import relay
from tvm.relay import op
from tvm.relay import ScopeBuilder
from tvm.relay.prelude import Prelude
from tvm.relay.testing.lstm import lstm_cell
import numpy as np

def generate_random_tensor(ty):
    return tvm.nd.array(np.random.rand(*[int(int_imm)
                                         for int_imm in ty.shape]).astype(ty.dtype))

# returns a function implementing an lstm
def lstm_definition(p, batch_size, num_hidden, dtype):
    input_type = relay.TensorType((batch_size, num_hidden), dtype)
    weight_type = relay.TensorType((4*num_hidden, num_hidden), dtype)
    bias_type = relay.TensorType((4*num_hidden,), dtype)
    state_type = relay.TupleType([input_type, input_type])
    cell_type = relay.TupleType([input_type, state_type])
    state_var_type = relay.TupleType([p.l(input_type), state_type])
    input_list = relay.Var('input_list', p.l(input_type))
    init_states = relay.Var('init_states', state_type)
    cell_fn = lstm_cell(num_hidden, batch_size, dtype, "lstm_cell")
    i2h_weight = relay.Var('i2h_weight', weight_type)
    i2h_bias = relay.Var('i2h_bias', bias_type)
    h2h_weight = relay.Var('h2h_weight', weight_type)
    h2h_bias = relay.Var('h2h_bias', bias_type)
    state_var = relay.Var('state_var', state_var_type)
    input_var = relay.Var('input_var', input_type)
    cell_out = relay.Var('cell_out', cell_type)
    iteration = relay.Function(
        [state_var, input_var],
        relay.Let(cell_out,
                  cell_fn(input_var, relay.TupleGetItem(state_var, 1),
                          i2h_weight, i2h_bias,
                          h2h_weight, h2h_bias),
                  relay.Tuple([
                      p.cons(relay.TupleGetItem(cell_out, 0),
                             relay.TupleGetItem(state_var, 0)),
                      relay.TupleGetItem(cell_out, 1)
                  ])),
        state_var_type)
    fold_res = relay.Var('fold_res', state_var_type)
    return relay.Function(
        [i2h_weight, i2h_bias, h2h_weight, h2h_bias, init_states, input_list],
        relay.Let(
            fold_res,
            p.foldl(iteration, relay.Tuple([p.nil(), init_states]), input_list),
            relay.Tuple([p.rev(relay.TupleGetItem(fold_res, 0)), relay.TupleGetItem(fold_res, 1)])),
        state_var_type)


# Produces a function that takes in a sequence of tensors of shape (batch_size, *orig_dims),
# reshapes each tensor to (batch_size, *reshape_dims), and concatenates them all together,
# with a final result of size (?, *reshape dims).
# The reshaping is done primarily to satisfy max_pool2d.
def combine_sequence(p, batch_size, orig_dims, reshape_dims, dtype='float32'):
    dyn_shape = tuple([relay.Any(), *reshape_dims])
    reshaped = tuple([batch_size, *reshape_dims])
    item_type = relay.TensorType(tuple([batch_size, *orig_dims]), dtype)
    acc_type = relay.TensorType(dyn_shape, dtype)
    seq = relay.Var('seq', p.l(item_type))
    h = relay.Var('h')
    t = relay.Var('t')
    x = relay.Var('x', acc_type)
    y = relay.Var('y', item_type)
    match = relay.Match(seq, [
        relay.Clause(
            relay.PatternConstructor(p.cons, [
                relay.PatternVar(h), relay.PatternVar(t)]),
            p.foldl(relay.Function([x, y],
                                   op.concatenate([x, op.reshape(y, reshaped)], 0),
                                   acc_type),
                    op.reshape(h, reshaped), t))
    ], complete=False)
    return relay.Function([seq], match, acc_type)


# simplification: start with a sequence, apply LSTM, combine outputs into one tensor,
# then apply maxpool
def audio_encoder_func(batch_size, num_hidden, reshape_dims, dtype):
    mod = tvm.IRModule()
    p = Prelude(mod)

    input_type = relay.TensorType((batch_size, num_hidden), dtype)
    weight_type = relay.TensorType((4*num_hidden, num_hidden), dtype)
    bias_type = relay.TensorType((4*num_hidden,), dtype)
    state_type = relay.TupleType([input_type, input_type])

    # parameters
    src = relay.Var('src', p.l(input_type))
    init_states = relay.Var('init_states', state_type)
    i2h_weight = relay.Var('i2h_weight', weight_type)
    i2h_bias = relay.Var('i2h_bias', bias_type)
    h2h_weight = relay.Var('h2h_weight', weight_type)
    h2h_bias = relay.Var('h2h_bias', bias_type)
    state_var_type = relay.TupleType([p.l(input_type), state_type])

    mod['lstm'] = lstm_definition(p, batch_size, num_hidden, dtype)
    mod['combine_seq'] = combine_sequence(p, batch_size, [num_hidden], reshape_dims, dtype)

    sb = relay.ScopeBuilder()
    lstm_res = relay.Var('lstm_res', state_var_type)
    sb.let(lstm_res, mod.get_global_var('lstm')(i2h_weight, i2h_bias,
                                                h2h_weight, h2h_bias,
                                                init_states, src))
    combined_res = relay.Var('combined_res')
    sb.let(combined_res, mod.get_global_var('combine_seq')(relay.TupleGetItem(lstm_res, 0)))
    # now apply maxpool, change params as you like
    sb.ret(op.nn.max_pool2d(combined_res,
                            pool_size=(1, 1),
                            strides=(1, 1),
                            padding=(0, 0),
                            layout='NCHW',
                            ceil_mode=False))
    mod['audio_enc'] = relay.Function([src, init_states,
                                       i2h_weight, i2h_bias, h2h_weight, h2h_bias], sb.get())
    return mod, p


def main():
    batch_size, num_hidden = 10, 25 # made-up numbers
    reshape_dims = [5, 1, 5] # also chozen pretty haphazardly
    dtype = 'float32'

    mod, p = audio_encoder_func(batch_size, num_hidden, reshape_dims, dtype)
    inp_ty = relay.TensorType((batch_size, num_hidden), dtype)
    weight_type = relay.TensorType((4*num_hidden, num_hidden), dtype)
    bias_type = relay.TensorType((4*num_hidden,), dtype)
    state_type = relay.TupleType([inp_ty, inp_ty])

    x = relay.Var('x', inp_ty)
    y = relay.Var('y', inp_ty)
    z = relay.Var('z', inp_ty)
    init_state0 = relay.Var('init_state0', inp_ty)
    init_state1 = relay.Var('init_state1', inp_ty)
    i2h_weight = relay.Var('i2h_weight', weight_type)
    i2h_bias = relay.Var('i2h_bias', bias_type)
    h2h_weight = relay.Var('h2h_weight', weight_type)
    h2h_bias = relay.Var('h2h_bias', bias_type)

    # wraps x, y, z into a list to avoid having to construct a vm list or tuple literals
    mod['main'] = relay.Function([x, y, z,
                                  init_state0, init_state1,
                                  i2h_weight, i2h_bias, h2h_weight, h2h_bias],
                                 mod.get_global_var('audio_enc')(
                                     p.cons(x, p.cons(y, p.cons(z, p.nil()))),
                                     relay.Tuple([init_state0, init_state1]),
                                     i2h_weight, i2h_bias, h2h_weight, h2h_bias))

    ex = relay.create_executor('vm', mod=mod, ctx=tvm.cpu(), target='llvm')
    out = ex.evaluate()(generate_random_tensor(inp_ty), generate_random_tensor(inp_ty), generate_random_tensor(inp_ty),
                        generate_random_tensor(inp_ty), generate_random_tensor(inp_ty),
                        generate_random_tensor(weight_type), generate_random_tensor(bias_type),
                        generate_random_tensor(weight_type), generate_random_tensor(bias_type))
    print(out)


if __name__ == '__main__':
    main()
    

def @rnn(%i2h_weight: Tensor[(4, 1), float32], %i2h_bias: Tensor[(4), float32], %h2h_weight: Tensor[(4, 1), float32], %h2h_bias: Tensor[(4), float32], %init_states: (Tensor[(1, 1), float32], Tensor[(1, 1), float32]), %input_list: List[Tensor[(1, 1), float32]]) -> (List[Tensor[(1, 1), float32]], (Tensor[(1, 1), float32], Tensor[(1, 1), float32])) {
  %18 = fn (%state_var: (List[Tensor[(1, 1), float32]], (Tensor[(1, 1), float32], Tensor[(1, 1), float32])), %input_var: Tensor[(1, 1), float32]) -> (List[Tensor[(1, 1), float32]], (Tensor[(1, 1), float32], Tensor[(1, 1), float32])) {
    %0 = %state_var.1;
    %13 = fn (%inputs: Tensor[(1, 1), float32], %states: (Tensor[(1, 1), float32], Tensor[(1, 1), float32]), %i2h_weight1: Tensor[(4, 1), float32], %i2h_bias1: Tensor[(4), float32], %h2h_weight1: Tensor[(4, 1), float32], %h2h_bias1: Tensor[(4), float32]) -> (Tensor[(1, 1), float32], (Tensor[(1, 1), float32], Tensor[(1, 1), float32])) {
      %1 = nn.dense(%inputs, %i2h_weight1, units=4) /* ty=Tensor[(1, 4), float32] */;
      let %i2h: Tensor[(1, 4), float32] = nn.bias_add(%1, %i2h_bias1, axis=-1) /* ty=Tensor[(1, 4), float32] */;
      %2 = %states.0;
      %3 = nn.dense(%2, %h2h_weight1, units=4) /* ty=Tensor[(1, 4), float32] */;
      let %h2h: Tensor[(1, 4), float32] = nn.bias_add(%3, %h2h_bias1, axis=-1) /* ty=Tensor[(1, 4), float32] */;
      let %gates: Tensor[(1, 4), float32] = add(%i2h, %h2h) /* ty=Tensor[(1, 4), float32] */;
      let %slice_gates: (Tensor[(1, 1), float32], Tensor[(1, 1), float32], Tensor[(1, 1), float32], Tensor[(1, 1), float32]) = split(%gates, indices_or_sections=4, axis=1) /* ty=(Tensor[(1, 1), float32], Tensor[(1, 1), float32], Tensor[(1, 1), float32], Tensor[(1, 1), float32]) */;
      %4 = %slice_gates.0;
      let %in_gate: Tensor[(1, 1), float32] = sigmoid(%4) /* ty=Tensor[(1, 1), float32] */;
      %5 = %slice_gates.1;
      let %forget_gate: Tensor[(1, 1), float32] = sigmoid(%5) /* ty=Tensor[(1, 1), float32] */;
      %6 = %slice_gates.2;
      let %in_transform: Tensor[(1, 1), float32] = tanh(%6) /* ty=Tensor[(1, 1), float32] */;
      %7 = %slice_gates.3;
      let %out_gate: Tensor[(1, 1), float32] = sigmoid(%7) /* ty=Tensor[(1, 1), float32] */;
      %8 = %states.1;
      %9 = multiply(%forget_gate, %8) /* ty=Tensor[(1, 1), float32] */;
      %10 = multiply(%in_gate, %in_transform) /* ty=Tensor[(1, 1), float32] */;
      let %next_c: Tensor[(1, 1), float32] = add(%9, %10) /* ty=Tensor[(1, 1), float32] */;
      %11 = tanh(%next_c) /* ty=Tensor[(1, 1), float32] */;
      let %next_h: Tensor[(1, 1), float32] = multiply(%out_gate, %11) /* ty=Tensor[(1, 1), float32] */;
      %12 = (%next_h, %next_c);
      let %ret: (Tensor[(1, 1), float32], (Tensor[(1, 1), float32], Tensor[(1, 1), float32])) = (%next_h, %12);
      %ret
    };
    let %cell_out: (Tensor[(1, 1), float32], (Tensor[(1, 1), float32], Tensor[(1, 1), float32])) = %13(%input_var, %0, %i2h_weight, %i2h_bias, %h2h_weight, %h2h_bias) /* ty=(Tensor[(1, 1), float32], (Tensor[(1, 1), float32], Tensor[(1, 1), float32])) */;
    %14 = %cell_out.0;
    %15 = %state_var.0;
    %16 = Cons(%14, %15) /* ty=List[Tensor[(1, 1), float32]] */;
    %17 = %cell_out.1;
    (%16, %17)
  };
  %19 = Nil /* ty=List[Tensor[(1, 1), float32]] */;
  %20 = (%19, %init_states);
  let %fold_res: (List[Tensor[(1, 1), float32]], (Tensor[(1, 1), float32], Tensor[(1, 1), float32])) = @foldl(%18, %20, %input_list) /* ty=(List[Tensor[(1, 1), float32]], (Tensor[(1, 1), float32], Tensor[(1, 1), float32])) */;
  %21 = %fold_res.0;
  %22 = @rev(%21) /* ty=List[Tensor[(1, 1), float32]] */;
  %23 = %fold_res.1;
  (%22, %23)
}
