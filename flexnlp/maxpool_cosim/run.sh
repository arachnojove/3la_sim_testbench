rm -f *.o *.out
export FLEX_SIM_DIR=/home/yl29/3LA/flexnlp-ila/build/sim_model
export FLEX_MODEL_DIR=$FLEX_SIM_DIR/build/CMakeFiles/flex_sim.dir/src
export FLEX_APP_DIR=$FLEX_SIM_DIR/build/CMakeFiles/flex_sim.dir/app
export FLEX_MODEL_UF_DIR=$FLEX_SIM_DIR/build/CMakeFiles/flex_sim.dir/extern

export RELAY_SIM_DIR=~/3LA/relay-ila/build/sim_model
export RELAY_MODEL_DIR=$RELAY_SIM_DIR/build/CMakeFiles/relay_sim.dir/src
export RELAY_MODEL_UF_DIR=$RELAY_SIM_DIR/build/CMakeFiles/relay_sim.dir/extern

export SYSTEMC_DIR=/home/yl29/local/systemc-2.3.3

export FLEX_ROOT=/home/yl29/3LA/FlexNLP
export CONNECTION_HOME=$FLEX_ROOT/matchlib/matchlib_connections
export RAPIDJSON_HOME=$FLEX_ROOT/matchlib/rapidjson
export MATCHLIB_HOME=$FLEX_ROOT/matchlib/cmod
export FLEX_HOME=$FLEX_ROOT/cmod

export BOOST_HOME=/usr/local
export SYSTEMC_HOME=$SYSTEMC_DIR
export CATAPULT_HOME=/home/yl29/3LA/MentorGraphics2020/Catapult_Synthesis_10.5a-871028

export INCDIR="-I. -I$SYSTEMC_HOME/include -I$BOOST_HOME/include -I$CATAPULT_HOME/Mgc_home/shared/include "
export INCDIR+="-I$CONNECTION_HOME/include "
export INCDIR+="-I$RAPIDJSON_HOME/include "
export INCDIR+="-I$MATCHLIB_HOME/include "
export INCDIR+="-I$FLEX_HOME/include "
export INCDIR+="-I$FLEX_HOME "

export INCDIR+="-I$FLEX_SIM_DIR/include "
export INCDIR+="-I$RELAY_SIM_DIR/include "

export LIBDIR="-L. -L$SYSTEMC_HOME/lib64 -Wl,-rpath=$SYSTEMC_HOME/lib64 -L$BOOST_HOME/lib"

export CFLAGS="$INCDIR $LIBDIR -Wall -Wno-unknown-pragmas -Wno-virtual-move-assign -Wno-unused-local-typedefs -std=c++11"
export CFLAGS+=" -DSC_INCLUDE_DYNAMIC_PROCESSES -DCONNECTIONS_ACCURATE_SIM"
export CFLAGS+=" -DHLS_CATAPULT"

export CFLAGS_UF=$CFLAGS" -g -c -o uninterpreted_func.o"
#export CFLAGS_UF_RELAY=$CFLAGS" -g -c -o uninterpreted_func_relay.o"

export CFLAGS_T=$CFLAGS" -g -o maxp_testbench.out"

export LIBS=" -lstdc++ -lsystemc -lm -lpthread -lboost_chrono -lboost_system"

echo $CFLAGS_UF

g++ $CFLAGS_UF uninterpreted_func.cc $LIBS
#g++ $CFLAGS_UF_RELAY uninterpreted_func_relay.cc $LIBS

g++ $CFLAGS_T maxpool_sim_main.cc $FLEX_MODEL_DIR/*.o $RELAY_MODEL_DIR/*.o *.o $LIBS

