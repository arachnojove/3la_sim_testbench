rm -f *.o
export FLEX_SIM_DIR=/u/yl29/3LA/flexnlp_sim/flexnlp-ila/build/sim_model
export FLEX_MODEL_DIR=$FLEX_SIM_DIR/build/CMakeFiles/flex_sim.dir/src
# dir of uninterpreted functions of flexNLP
export FLEX_MODEL_UF_DIR=$FLEX_SIM_DIR/build/CMakeFiles/flex_sim.dir/extern

export RELAY_SIM_DIR=/u/yl29/3LA/relay_sim/relay-ila/build/sim_model
export RELAY_MODEL_DIR=$RELAY_SIM_DIR/build/CMakeFiles/relay_sim.dir/src
export RELAY_MODEL_UF_DIR=$RELAY_SIM_DIR/build/CMakeFiles/relay_sim.dir/extern

export SYSTEMC_DIR=/u/yl29/local/systemc-2.3.3
g++ -I $SYSTEMC_DIR/include -L $SYSTEMC_DIR/lib64 -Wl,-rpath=$SYSTEMC_DIR/lib64 -std=c++11 -g -o maxpool_sim_main_debug.o maxpool_sim_main_debug.cc $FLEX_MODEL_DIR/*.o $FLEX_MODEL_UF_DIR/*.o $RELAY_MODEL_DIR/*.o $RELAY_MODEL_UF_DIR/*.o -lsystemc
