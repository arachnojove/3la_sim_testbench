CC = g++
CFLAGS = -std=c++11 -g

INCDIR = -I. -I./json_helper
CURDIR = $(shell pwd)

CFLAGS += $(INCDIR)

all: input_gen 

clean:
	rm -rf *.out *.o

input_gen: input_gen.cc
	$(CC) $(CFLAGS) input_gen.cc -o input_gen.out
extract_sim_log: extract_sim_log.cc vivado_hls.log
	$(CC) $(CFLAGS) extract_sim_log.cc -o extract_sim_log.out

VTA_SIM_DIR = /home/leeoh/3la/vta-ila/build/sim_model
VTA_MODEL_DIR = $(VTA_SIM_DIR)/build/CMakeFiles/vta.dir/src

AP_INCLUDE = $(CURDIR)/ap_include
SYSTEMC_HOME = /home/leeoh/local/systemc-2.3.3
BOOST_HOME = /usr/local


INCDIR += -I$(VTA_SIM_DIR)/include
INCDIR += -I$(AP_INCLUDE)

INCDIR += -I$(SYSTEMC_HOME)/include
INCDIR += -I$(BOOST_HOME)/include

LIBDIR = -L. -L$(SYSTEMC_HOME)/lib -Wl,-rpath=$(SYSTEMC_HOME)/lib
LIBDIR += -L$(BOOST_HOME)/lib

LIBS = -lstdc++ -lsystemc -lboost_chrono -lboost_system

CFLAGS_VTA = $(INCDIR) $(LIBDIR) -std=c++11 -g

uninterpreted_func.o: uninterpreted_func.cc $(VTA_SIM_DIR)/include/*.h
	$(CC) $(CFLAGS_VTA) -c uninterpreted_func.cc -o uninterpreted_func.o $(LIBS)

testbench.out : uninterpreted_func.o testbench.cc $(VTA_MODEL_DIR)/*.o $(VTA_SIM_DIR)/include/*.h
	$(CC) $(CFLAGS_VTA) testbench.cc uninterpreted_func.o $(VTA_MODEL_DIR)/*.o -o testbench.out $(LIBS)
