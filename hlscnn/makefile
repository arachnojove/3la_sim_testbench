#simple makefile
CC = g++
CFLAGS = -std=c++11 -g

CUR_DIR = /home/yl20/3LA/testbench/hlscnn

INCDIR = -I. -I./json_helper

CFLAGS+= $(INCDIR)

all: extract_input

extract_input: extract_input_seq.cc
	$(CC) $(CFLAGS) extract_input_seq.cc -o extract_input_seq.out
