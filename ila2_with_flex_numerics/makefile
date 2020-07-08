#simple makefile
#MACROS
CC = g++
CFLAGS = -std=c++11

all: maxp_testbench.out attention_testbench.out layernorm_testbench.out gru_testbench.out rnn_testbench.out lstm_testbench.out

clean:
	rm -f *.out *.o

FLEX_SIM_DIR = /home/yl29/3LA/flexnlp-ila/build/sim_model
FLEX_MODEL_DIR = $(FLEX_SIM_DIR)/build/CMakeFiles/flex.dir/src
FLEX_APP_DIR = $(FLEX_SIM_DIR)/build/CMakeFiles/flex.dir/app

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

LIBDIR= -L. -L$(SYSTEMC_HOME)/lib -Wl,-rpath=$(SYSTEMC_HOME)/lib -L$(BOOST_HOME)/lib

CFLAGS_FLEX = $(INCDIR) $(LIBDIR) -Wall -Wno-unknown-pragmas -Wno-virtual-move-assign -Wno-unused-local-typedefs -std=c++11
CFLAGS_FLEX+=  -DSC_INCLUDE_DYNAMIC_PROCESSES -DCONNECTIONS_ACCURATE_SIM
CFLAGS_FLEX+=  -DHLS_CATAPULT

LIBS= -lstdc++ -lsystemc -lm -lpthread -lboost_chrono -lboost_system

TB_PATH = ./testbench

uninterpreted_func.o: uninterpreted_func.cc $(FLEX_SIM_DIR)/include/*
	$(CC) $(CFLAGS_FLEX) -c uninterpreted_func.cc -o uninterpreted_func.o $(LIBS)

maxp_testbench.out: uninterpreted_func.o $(TB_PATH)/maxp_testbench.cc $(FLEX_MODEL_DIR)/*.o 
	$(CC) $(CFLAGS_FLEX) $(TB_PATH)/maxp_testbench.cc uninterpreted_func.o $(FLEX_MODEL_DIR)/*.o -o maxp_testbench.out $(LIBS)

attention_testbench.out: uninterpreted_func.o $(TB_PATH)/attention_testbench.cc $(FLEX_MODEL_DIR)/*.o 
	$(CC) $(CFLAGS_FLEX) $(TB_PATH)/attention_testbench.cc uninterpreted_func.o $(FLEX_MODEL_DIR)/*.o -o attention_testbench.out $(LIBS)

layernorm_testbench.out: uninterpreted_func.o $(TB_PATH)/layernorm_testbench.cc $(FLEX_MODEL_DIR)/*.o 
	$(CC) $(CFLAGS_FLEX) $(TB_PATH)/layernorm_testbench.cc uninterpreted_func.o $(FLEX_MODEL_DIR)/*.o -o layernorm_testbench.out $(LIBS)

rnn_testbench.out: uninterpreted_func.o $(TB_PATH)/rnn_testbench.cc $(FLEX_MODEL_DIR)/*.o 
	$(CC) $(CFLAGS_FLEX) $(TB_PATH)/rnn_testbench.cc uninterpreted_func.o $(FLEX_MODEL_DIR)/*.o -o rnn_testbench.out $(LIBS)

gru_testbench.out: uninterpreted_func.o $(TB_PATH)/gru_testbench.cc $(FLEX_MODEL_DIR)/*.o 
	$(CC) $(CFLAGS_FLEX) $(TB_PATH)/gru_testbench.cc uninterpreted_func.o $(FLEX_MODEL_DIR)/*.o -o gru_testbench.out $(LIBS)

lstm_testbench.out: uninterpreted_func.o $(TB_PATH)/lstm_testbench.cc $(FLEX_MODEL_DIR)/*.o 
	$(CC) $(CFLAGS_FLEX) $(TB_PATH)/lstm_testbench.cc uninterpreted_func.o $(FLEX_MODEL_DIR)/*.o -o lstm_testbench.out $(LIBS)
