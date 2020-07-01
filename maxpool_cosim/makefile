#simple makefile
#MACROS
CC = g++
CFLAGS = -std=c++11

all: test_gen maxp_testbench.out
# maxp_test_gen: maxp_test_gen.out
# maxp_testbench: maxp_testbench.out
clean: 
	rm -f *.out *.o

#dependency of the files
test_gen: maxp_test_gen.cc 
	$(CC) $(CFLAGS) maxp_test_gen.cc -o maxp_test_gen.out

json_gen: prog_frag_gen.cc
	$(CC) $(CFLAGS) prog_frag_gen.cc -o json_gen.out

FLEX_SIM_DIR = /home/yl29/3LA/flexnlp-ila/build/sim_model
FLEX_MODEL_DIR = $(FLEX_SIM_DIR)/build/CMakeFiles/flex_sim.dir/src
FLEX_APP_DIR = $(FLEX_SIM_DIR)/build/CMakeFiles/flex_sim.dir/app
FLEX_MODEL_UF_DIR = $(FLEX_SIM_DIR)/build/CMakeFiles/flex_sim.dir/extern

RELAY_SIM_DIR = /home/yl29/3LA/relay-ila/build/sim_model
RELAY_MODEL_DIR = $(RELAY_SIM_DIR)/build/CMakeFiles/relay_sim.dir/src
RELAY_MODEL_UF_DIR = $(RELAY_SIM_DIR)/build/CMakeFiles/relay_sim.dir/extern

SYSTEMC_DIR = /home/yl29/local/systemc-2.3.3

FLEX_ROOT = /home/yl29/3LA/FlexNLP
CONNECTION_HOME = $(FLEX_ROOT)/matchlib/matchlib_connections
RAPIDJSON_HOME = $(FLEX_ROOT)/matchlib/rapidjson
MATCHLIB_HOME = $(FLEX_ROOT)/matchlib/cmod
FLEX_HOME = $(FLEX_ROOT)/cmod

BOOST_HOME=/usr/local
SYSTEMC_HOME=$(SYSTEMC_DIR)
CATAPULT_HOME=/home/yl29/3LA/MentorGraphics2020/Catapult_Synthesis_10.5a-871028

INCDIR = -I. -I$(SYSTEMC_HOME)/include -I$(BOOST_HOME)/include -I$(CATAPULT_HOME)/Mgc_home/shared/include 
INCDIR+= -I$(CONNECTION_HOME)/include 
INCDIR+= -I$(RAPIDJSON_HOME)/include 
INCDIR+= -I$(MATCHLIB_HOME)/include 
INCDIR+= -I$(FLEX_HOME)/include 
INCDIR+= -I$(FLEX_HOME) 

INCDIR+= -I$(FLEX_SIM_DIR)/include 
INCDIR+= -I$(RELAY_SIM_DIR)/include 

LIBDIR= -L. -L$(SYSTEMC_HOME)/lib64 -Wl,-rpath=$(SYSTEMC_HOME)/lib64 -L$(BOOST_HOME)/lib

CFLAGS_FLEX = $(INCDIR) $(LIBDIR) -Wall -Wno-unknown-pragmas -Wno-virtual-move-assign -Wno-unused-local-typedefs -std=c++11
CFLAGS_FLEX+=  -DSC_INCLUDE_DYNAMIC_PROCESSES -DCONNECTIONS_ACCURATE_SIM
CFLAGS_FLEX+=  -DHLS_CATAPULT

LIBS= -lstdc++ -lsystemc -lm -lpthread -lboost_chrono -lboost_system

maxp_testbench.out: uninterpreted_func.o maxpool_sim_main.cc $(FLEX_MODEL_DIR)/*.o $(RELAY_MODEL_DIR)/*.o
	$(CC) $(CFLAGS_FLEX) maxpool_sim_main.cc uninterpreted_func.o $(FLEX_MODEL_DIR)/*.o $(RELAY_MODEL_DIR)/*.o -o maxp_testbench.out $(LIBS)

uninterpreted_func.o: uninterpreted_func.cc
	$(CC) $(CFLAGS_FLEX) -c uninterpreted_func.cc -o uninterpreted_func.o $(LIBS)






