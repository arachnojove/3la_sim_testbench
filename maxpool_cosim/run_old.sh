rm -f *.o
g++ -I $HOME/local/systemc-2.3.1a/include -L $HOME/local/systemc-2.3.1a/lib-linux64 -Wl,-rpath=$HOME/local/systemc-2.3.1a/lib-linux64 -std=c++11 -o maxpool_sim_main.o maxpool_sim_main.cc ../flexnlp_sim/FlexNLP-ila/build/sim_model/*.o ../relay_sim/relay-ila/build/sim_model/*.o -lsystemc
