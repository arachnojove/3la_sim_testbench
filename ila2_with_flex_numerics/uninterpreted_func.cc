#include "systemc.h"
#include "flex_sim.h"

#include "SM6Spec.h"
#include "AdpfloatSpec.h"
#include "AdpfloatUtils.h"

sc_biguint<8> flex_sim::signed_gt(sc_biguint<8> arg_0, sc_biguint<8> arg_1) {

  ac_int<8,false> arg_0_ac = arg_0.to_uint();
  ac_int<8,false> arg_1_ac = arg_1.to_uint();
  AdpfloatType<spec::kAdpfloatWordWidth,spec::kAdpfloatExpWidth> in_a = arg_0_ac;
  AdpfloatType<spec::kAdpfloatWordWidth,spec::kAdpfloatExpWidth> in_b = arg_1_ac;
  AdpfloatType<spec::kAdpfloatWordWidth,spec::kAdpfloatExpWidth> out_tmp;
  adpfloat_max(in_a, in_b, out_tmp);

  ac_int<8,false> result_ac = out_tmp.to_rawbits();
  sc_biguint<8> result = result_ac.to_uint();

  return result;
}

sc_biguint<20> flex_sim::PEActEadd(sc_biguint<20> arg_0, sc_biguint<20> arg_1) {
  // TODO: need to implement signed Add
  sc_bigint<20> arg_0_s = arg_0;
  sc_bigint<20> arg_1_s = arg_1;

  ActScalarType arg_0_ac = arg_0_s.to_int();
  ActScalarType arg_1_ac = arg_1_s.to_int();

  ActScalarType out_tmp = arg_0_ac + arg_1_ac;

  sc_bigint<20> result_s = out_tmp.to_int();
  sc_biguint<20> result = result_s;
  
  return result;
}

sc_biguint<20> flex_sim::PEActEmul(sc_biguint<20> arg_0, sc_biguint<20> arg_1) {
  // TODO: implement signed multiply
  sc_bigint<20> arg_0_s = arg_0;
  sc_bigint<20> arg_1_s = arg_1;
  
  ActScalarType arg_0_ac = arg_0_s.to_int();
  ActScalarType arg_1_ac = arg_1_s.to_int();
  
  NVINTW(2*spec::kActWordWidth) tmp;
  tmp = arg_0_ac * arg_1_ac;
  tmp = nvhls::right_shift<NVINTW(2*spec::kActWordWidth)>(tmp, spec::kActNumFrac);
  ActScalarType out_tmp = nvhls::get_slc<spec::kActWordWidth>(tmp, 0);

  sc_bigint<20> result_s = out_tmp.to_int();
  sc_biguint<20> result = result_s;

  return result;
}

sc_biguint<20> flex_sim::PEActSigmoid(sc_biguint<20> arg_0) {
  // TODO: implement Sigmoid
  sc_bigint<20> arg_0_s = arg_0;
  
  ActScalarType arg_0_ac = arg_0_s.to_int();
  ActScalarType out_tmp;

  ac_fixed<spec::kActWordWidth, spec::kActNumInt, true, AC_TRN, AC_WRAP> in_ac; 
  ac_fixed<spec::kActWordWidth, spec::kActNumInt, false, AC_TRN, AC_WRAP> out_ac;
  
  in_ac.set_slc(0, arg_0_ac);
  out_ac = ac_math::ac_sigmoid_pwl
            <ac_fixed<spec::kActWordWidth, spec::kActNumInt, false, AC_TRN, AC_WRAP> >(in_ac);

  out_tmp.set_slc(0, nvhls::get_slc<spec::kActWordWidth>(out_ac, 0));

  sc_bigint<20> result_s = out_tmp.to_int();
  sc_biguint<20> result = result_s;

  return result;
}

sc_biguint<20> flex_sim::PEActTanh(sc_biguint<20> arg_0) {
  // TODO: implement Tanh
  sc_bigint<20> arg_0_s = arg_0;

  ActScalarType arg_0_ac = arg_0_s.to_int();
  ActScalarType out_tmp;

  ac_fixed<spec::kActWordWidth, spec::kActNumInt, true, AC_TRN, AC_WRAP> in_ac;
  ac_fixed<spec::kActWordWidth, spec::kActNumInt, false, AC_TRN, AC_WRAP> out_ac;

  in_ac.set_slc(0, arg_0_ac);

  out_ac = ac_math::ac_tanh_pwl
             <ac_fixed<spec::kActWordWidth, spec::kActNumInt, false, AC_TRN, AC_WRAP> >(in_ac);

  out_tmp.set_slc(0, nvhls::get_slc<spec::kActWordWidth>(out_ac, 0));
  
  sc_bigint<20> result_s = out_tmp.to_int();
  sc_biguint<20> result = result_s;

  return result;
}

sc_biguint<20> flex_sim::PEActRelu(sc_biguint<20> arg_0) {
  // TODO: implement Relu
  sc_bigint<20> arg_0_s = arg_0;

  ActScalarType arg_0_ac = arg_0_s.to_int();
  ActScalarType out_tmp;

  if (arg_0_ac < 0)
      out_tmp = 0;
  else 
      out_tmp = arg_0_ac;

  sc_bigint<20> result_s = out_tmp.to_int();
  sc_biguint<20> result = result_s;

  return result;
}

sc_biguint<20> flex_sim::PEActOnex(sc_biguint<20> arg_0) {
  // TODO: implement Onex
  sc_bigint<20> arg_0_s = arg_0;

  ActScalarType arg_0_ac = arg_0_s.to_int();
  ActScalarType out_tmp;

  out_tmp = (1<<spec::kActNumFrac) - arg_0_ac;

  sc_bigint<20> result_s = out_tmp.to_int();
  sc_biguint<20> result = result_s;

  return result;
}

sc_biguint<20> flex_sim::Adptflow2Fixed(sc_biguint<8> arg_0, sc_biguint<3> arg_1) {
  // TODO: implement adptflow to fixed
  sc_biguint<20> result = 0xabcd;

  return result;
}

sc_biguint<8> flex_sim::Fixed2Adaptflow(sc_biguint<20> arg_0, sc_biguint<3> arg_1) {
  // TODO: implement fixed to adaptflow
  sc_biguint<8> result = 0xab;

  return result;
}








