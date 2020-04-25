rm -f *.o
export FLEX_SIM_DIR=/home/yl29/3LA/flexnlp-ila/build/sim_model
export FLEX_MODEL_DIR=$FLEX_SIM_DIR/build/CMakeFiles/flex_sim.dir/src
export FLEX_MODEL_UF_DIR=$FLEX_SIM_DIR/build/CMakeFiles/flex_sim.dir/extern

export SYSTEMC_DIR=/home/yl29/local/systemc-2.3.3
g++ -I $SYSTEMC_DIR/include -L $SYSTEMC_DIR/lib -Wl,-rpath=$SYSTEMC_DIR/lib64 -std=c++11 -g -o testbench.o testbench.cc $FLEX_MODEL_DIR/*.o $FLEX_MODEL_UF_DIR/*.o -lsystemc
