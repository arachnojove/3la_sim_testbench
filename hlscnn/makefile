#simple makefile
CC = g++
CFLAGS = -std=c++11 -g

CUR_DIR = /home/yl20/3LA/testbench/hlscnn

INCDIR = -I. -I./json_helper

CFLAGS+= $(INCDIR)

all: extract_input

clean:
	rm -f *.out *.o

extract_input: extract_input_seq.cc
	$(CC) $(CFLAGS) extract_input_seq.cc -o extract_input_seq.out

HLSCNN_SIM_DIR = /home/yl29/3LA/hlscnn-ila/build/sim_model
HLSCNN_MODEL_DIR = $(HLSCNN_SIM_DIR)/build/CMakeFiles/hlscnn.dir/src

BOOST_HOME = /usr/local
SYSTEMC_HOME = /home/yl29/local/systemc-2.3.3


INCDIR+= -I$(SYSTEMC_HOME)/include -I$(BOOST_HOME)/include 
INCDIR+= -I$(HLSCNN_SIM_DIR)/include


LIBDIR = -L. -L$(SYSTEMC_HOME)/lib -Wl,-rpath=$(SYSTEMC_HOME)/lib -L$(BOOST_HOME)/lib

CFLAGS_HLSCNN = $(INCDIR) $(LIBDIR) -std=c++11 -g

LIBS = -lstdc++ -lsystemc -lm -lpthread -lboost_chrono -lboost_system

uninterpreted_func.o: uninterpreted_func.cc $(HLSCNN_SIM_DIR)/include/*
	$(CC) $(CFLAGS_HLSCNN) -c uninterpreted_func.cc -o uninterpreted_func.o $(LIBS)

conv_testbench.out: uninterpreted_func.o conv_testbench.cc $(HLSCNN_MODEL_DIR)/*.o
	$(CC) $(CFLAGS_HLSCNN) conv_testbench.cc uninterpreted_func.o $(HLSCNN_MODEL_DIR)/*.o -o conv_testbench.out $(LIBS)

