#include <systemc>
#include <iostream>
#include <sstream>
#include <fstream>
#include <sstream>

#include "nlohmann/json.hpp"
#include "hlscnn.h"

using json = nlohmann::json;

SC_MODULE(Source) {
  sc_in<bool> clk{"clk"};

  sc_out< sc_biguint<1> > hlscnn_if_wr;
  sc_out< sc_biguint<1> > hlscnn_if_rd;
  sc_out< sc_biguint<32> > hlscnn_addr_in;
  sc_out< sc_biguint<8> > hlscnn_data_in[16];

  sc_out< sc_biguint<1> > input_done;

  SC_CTOR(Source) {
    SC_THREAD(source_input);
    sensitive << clk.pos();
  }

  void source_input() {
    // reset the port
    hlscnn_if_wr = 0;
    hlscnn_if_rd = 0;
    hlscnn_addr_in = 0;
    for (auto i = 0; i < 16; i++) {
      hlscnn_data_in[i] = 0;
    }

    input_done = 0;
    // read program fragment from file
    std::ifstream fin;
    fin.open("./test_input_conv.json", ios::in);
    
    //parse the json file
    json cmd_seq;
    fin >> cmd_seq;

    // pass the command to the ports
    for (int i = 0; i < cmd_seq["program fragment"].size(); i++) {
      hlscnn_if_rd = std::stoi(cmd_seq["program fragment"][i]["is_rd"].get<std::string>(), nullptr, 16);
      hlscnn_if_wr = std::stoi(cmd_seq["program fragment"][i]["is_wr"].get<std::string>(), nullptr, 16);
      // fetch the address
      std::string addr = cmd_seq["program fragment"][i]["addr"].get<std::string>();
      hlscnn_addr_in = std::stoi(addr, nullptr, 16);
      // extract each data byte from data
      std::string data = cmd_seq["program fragment"][i]["data"].get<std::string>();
      for (int j = 0; j < 16; j++) {
        hlscnn_data_in[j] = std::stoi(data.substr(30-2*j,2), nullptr, 16);
      }
      wait(10, SC_NS);
    }

    input_done = 1;
}

};

SC_MODULE(testbench) {
  // SC_HAS_PROCESS(testbench);
  hlscnn hlscnn_inst;
  Source src;

  sc_clock clk;
  sc_signal< sc_biguint<1> > hlscnn_if_wr_signal;
  sc_signal< sc_biguint<1> > hlscnn_if_rd_signal;
  sc_signal< sc_biguint<32> > hlscnn_addr_signal;
  sc_signal< sc_biguint<8> > hlscnn_data_signal[16];

  sc_signal< sc_biguint<1> > input_done;

  SC_CTOR(testbench) :
    clk("clk", 1, SC_NS),
    hlscnn_inst("hlscnn_inst"),
    src("source")
  {
    // binding the signals
    src.clk(clk);
    src.hlscnn_if_rd(hlscnn_if_rd_signal);
    src.hlscnn_if_wr(hlscnn_if_wr_signal);
    src.hlscnn_addr_in(hlscnn_addr_signal);
    for (int i = 0; i < 16; i++) {
      src.hlscnn_data_in[i](hlscnn_data_signal[i]);
    }
    src.input_done(input_done);

    // // connecting signals to hlscnn
    hlscnn_inst.hlscnn_top_slave_if_rd_in(hlscnn_if_rd_signal);
    hlscnn_inst.hlscnn_top_slave_if_wr_in(hlscnn_if_wr_signal);
    hlscnn_inst.hlscnn_top_slave_addr_in_in(hlscnn_addr_signal);
    hlscnn_inst.hlscnn_top_slave_data_in_0_in(hlscnn_data_signal[0]);
    hlscnn_inst.hlscnn_top_slave_data_in_1_in(hlscnn_data_signal[1]);
    hlscnn_inst.hlscnn_top_slave_data_in_2_in(hlscnn_data_signal[2]);
    hlscnn_inst.hlscnn_top_slave_data_in_3_in(hlscnn_data_signal[3]);
    hlscnn_inst.hlscnn_top_slave_data_in_4_in(hlscnn_data_signal[4]);
    hlscnn_inst.hlscnn_top_slave_data_in_5_in(hlscnn_data_signal[5]);
    hlscnn_inst.hlscnn_top_slave_data_in_6_in(hlscnn_data_signal[6]);
    hlscnn_inst.hlscnn_top_slave_data_in_7_in(hlscnn_data_signal[7]);
    hlscnn_inst.hlscnn_top_slave_data_in_8_in(hlscnn_data_signal[8]);
    hlscnn_inst.hlscnn_top_slave_data_in_9_in(hlscnn_data_signal[9]);
    hlscnn_inst.hlscnn_top_slave_data_in_10_in(hlscnn_data_signal[10]);
    hlscnn_inst.hlscnn_top_slave_data_in_11_in(hlscnn_data_signal[11]);
    hlscnn_inst.hlscnn_top_slave_data_in_12_in(hlscnn_data_signal[12]);
    hlscnn_inst.hlscnn_top_slave_data_in_13_in(hlscnn_data_signal[13]);
    hlscnn_inst.hlscnn_top_slave_data_in_14_in(hlscnn_data_signal[14]);
    hlscnn_inst.hlscnn_top_slave_data_in_15_in(hlscnn_data_signal[15]);


    SC_THREAD(run);
  }

  void run() {
    hlscnn_inst.instr_log.open("./instr_log_conv.txt", ofstream::out | ofstream::trunc);

    std::cout << "start running" << std::endl;
    std::cout << "*********** simulation start ***********" << std::endl;
    wait(10, SC_NS);

    while (input_done == 0) {
		  std::cout << "current simulation time: " << '\t' << sc_time_stamp() << "\r" << std::flush;
      wait(10, SC_NS);
    }

    wait(100000, SC_NS);
    std::cout << '\n' << std::endl;
    std::cout << "************* sc_stop **************" << std::endl;
    hlscnn_inst.instr_log.close();
    std::cout << "SPAD1: result: " << std::endl;
    for (int i = 0; i < 0x40; i++) {
    	std::cout << "addr: " << std::hex << 0x24000 + i*0x10 << '\t' << "data: ";
			for (int j = 0; j < 8; j++) {
      	sc_biguint<8> byte_0 = hlscnn_inst.hlscnn_scratch_pad_1[i*0x10 + 2*j];
        sc_biguint<8> byte_1 = hlscnn_inst.hlscnn_scratch_pad_1[i*0x10 + 2*j+1];
      	sc_biguint<16> data = byte_1;
        data = (data << 8) + byte_0;
        std::cout << std::hex << data.to_uint() << " ";
      }
      std::cout << std::endl;
    }
    sc_stop();
  }
};

int sc_main(int argc, char *argv[]) {
  std::cout << "test start" << std::endl;
  testbench tb("tb");
  sc_start();

  return 0;
}
