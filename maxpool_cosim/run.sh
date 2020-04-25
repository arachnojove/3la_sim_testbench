rm -f *.o
export FLEX_SIM_DIR=~/3LA/flexnlp-ila/build/sim_model
export FLEX_MODEL_DIR=$FLEX_SIM_DIR/build/CMakeFiles/flex_sim.dir/src
# dir of uninterpreted functions of flexNLP
export FLEX_MODEL_UF_DIR=$FLEX_SIM_DIR/build/CMakeFiles/flex_sim.dir/extern

export RELAY_SIM_DIR=~/3LA/relay-ila/build/sim_model
export RELAY_MODEL_DIR=$RELAY_SIM_DIR/build/CMakeFiles/relay_sim.dir/src
export RELAY_MODEL_UF_DIR=$RELAY_SIM_DIR/build/CMakeFiles/relay_sim.dir/extern

export SYSTEMC_DIR=~/local/systemc-2.3.3
g++ -I $SYSTEMC_DIR/include -L $SYSTEMC_DIR/lib -Wl,-rpath=$SYSTEMC_DIR/lib -std=c++11 -g -o maxpool_sim_main_debug.o maxpool_sim_main_debug.cc $FLEX_MODEL_DIR/*.o $FLEX_MODEL_UF_DIR/*.o $RELAY_MODEL_DIR/*.o $RELAY_MODEL_UF_DIR/*.o -lsystemc
