#include <systemc>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "flex_sim.h"
// source module of the testbench
// creating signals for flex_sim model
SC_MODULE(Source) {
  sc_in<bool> clk{"clk"};
  //sc_in<bool> rst;
  
  sc_out< sc_biguint<1> > flex_wr_in;
  sc_out< sc_biguint<1> > flex_rd_in;
  sc_out< sc_biguint<32> > flex_addr_in;
  sc_out< sc_biguint<8> > flex_data_in[16];

//  void source_input();
  SC_CTOR(Source) {
    SC_THREAD(source_input);
    sensitive << clk.pos();
    //reset_signal_is(rst, true);
  }

  void source_input() {
    // reset
    flex_wr_in = 0;
    flex_rd_in = 0;
    flex_addr_in = "0x0";
    for (int i = 0; i < 16; i++) {
      flex_data_in[i] = "0x0";
    }

    wait(10, SC_NS);

    std::ifstream fin;
    std::string mode, addr, data;
    std::string data_format;
    std::string temp;
    std::string hex_hdr = "0x";
    std::string addr_format;
    
    const char *addr_c;
    const char *data_byte_c;

    wait(10, SC_NS);

    //fin.open("./lstm_testbench_input.txt", ios::in);
    //fin.open("./testbench_input/axi_commands_1_LSTM_timesteps_clustered_weights_with_read_commands.txt", ios::in);
    fin.open("./testbench_input/axi_commands_for_kmeans_clustering_for_LSTM_4_timesteps_zero_first_enabled_4PEs.csv", ios::in);
    //fin.open("./testbench_input/axi_commands_for_2_lstm_64x64.txt", ios::in);

    while(std::getline(fin, temp, ',')) {
      std::getline(fin, mode, ',');
      std::getline(fin, addr, ',');
      std::getline(fin, data, '\n');

      // cout << mode << '\t' << addr << '\t' << data << endl;
      // set the WR/RD mode
      if (mode.compare("W") == 0) {
        flex_wr_in = 1;
        flex_rd_in = 0;
      } else {
        flex_wr_in = 0;
        flex_rd_in = 1;
      }
      // extract the address
      // addr = addr.substr(2);
      // addr_format = "0x00" + addr;
      // addr_c = addr_format.c_str();

      // remove the "0x" at the begining.
      //addr = addr.substr(2);
      addr_c = addr.c_str();
      //cout << "addr:" <<  strlen(addr_c) << '\t' << addr_c << endl;
      flex_addr_in = addr_c;

      // remove the possible '\r' at the end of the line
      if(int(data[data.size()-1]) == 13) {
        data = data.substr(0, data.size()-1);
      }
      // extract the data
      data_format.clear();
      if (data.length() <= 34) {
        data_format.append(34 - data.length(), '0');
        data_format.append(data.substr(2));
      } else {
        data_format.append(data.substr(data.length()-32));
      }

      std::string data_byte;
      for (int i = 0; i<16; i++) {
        data_byte = data_format.substr(30-2*i, 2);
        data_byte_c = ("0x0" + data_byte).c_str();
        int data_int = stoi(data_byte, nullptr, 16);
        //cout << hex << data_int << ' ';
        flex_data_in[i] = data_int;
      }
      //cout << endl;

      wait(10, SC_NS);
    }

    // cout << "source created for testbench" << endl;
  }
};

SC_MODULE(testbench) {
  SC_HAS_PROCESS(testbench);
  flex_sim flex;
  Source src;

  sc_clock clk;
  sc_signal< sc_biguint<1> > flex_wr_signal;
  sc_signal< sc_biguint<1> > flex_rd_signal;
  sc_signal< sc_biguint<32> > flex_addr_signal;
  sc_signal< sc_biguint<8> > flex_data_signal[16];

  testbench(sc_module_name name)
  : sc_module(name),
    clk("clk", 1, SC_NS),
    src("source"),
    flex("flexnlp")
  {
    // binding the signals from the source
    src.clk(clk);
    src.flex_wr_in(flex_wr_signal);
    src.flex_rd_in(flex_rd_signal);
    src.flex_addr_in(flex_addr_signal);
    for (int i = 0; i < 16; i++) {
      src.flex_data_in[i](flex_data_signal[i]);
    }

    // binding the signals for the model
    flex.flex_sim_if_axi_rd_in(flex_rd_signal);
    flex.flex_sim_if_axi_wr_in(flex_wr_signal);
    flex.flex_sim_addr_in_in(flex_addr_signal);
    flex.flex_sim_data_in_0_in(flex_data_signal[0]);
    flex.flex_sim_data_in_1_in(flex_data_signal[1]);
    flex.flex_sim_data_in_2_in(flex_data_signal[2]);
    flex.flex_sim_data_in_3_in(flex_data_signal[3]);
    flex.flex_sim_data_in_4_in(flex_data_signal[4]);
    flex.flex_sim_data_in_5_in(flex_data_signal[5]);
    flex.flex_sim_data_in_6_in(flex_data_signal[6]);
    flex.flex_sim_data_in_7_in(flex_data_signal[7]);
    flex.flex_sim_data_in_8_in(flex_data_signal[8]);
    flex.flex_sim_data_in_9_in(flex_data_signal[9]);
    flex.flex_sim_data_in_10_in(flex_data_signal[10]);
    flex.flex_sim_data_in_11_in(flex_data_signal[11]);
    flex.flex_sim_data_in_12_in(flex_data_signal[12]);
    flex.flex_sim_data_in_13_in(flex_data_signal[13]);
    flex.flex_sim_data_in_14_in(flex_data_signal[14]);
    flex.flex_sim_data_in_15_in(flex_data_signal[15]);

    flex.instr_log;
    SC_THREAD(run);
  }

  void run() {
    int i = 0;
    bool undone = true;
    int stop_addr = 0xdeaddead;
    std::ofstream fout;
    fout.open("./test_output/test_output_lstm.txt", ofstream::out | ofstream::trunc);
    flex.instr_log.open("./instr_log/instr_log_lstm.txt", ofstream::out | ofstream::trunc);
    
    wait(10, SC_NS);
    std::cout << "@" << sc_time_stamp() << " ********* simulation start *********" << std::endl;
    wait(10, SC_NS);

    while (undone) {
      if (flex.flex_sim_addr_in.to_int() == stop_addr) {
          undone = false;
          cout << "\n\n";
          cout << "@" << sc_time_stamp() << "******** testbench input commands end ***********" << endl;
      }
      cout << "current simulation time: " << sc_time_stamp() << "\r" << std::flush;
      //fout << "@ " << sc_time_stamp() << '\t';
      //fout << "is write? :" << '\t' << flex.flex_sim_if_axi_wr_in << '\t';
      //fout << "addr in:" << '\t' << hex << flex.flex_sim_addr_in << '\t';
      //fout << "data in:" << '\t';
      //for (int k=0; k < 16; k++) {
      //  fout << hex << flex_data_signal[15-k] << ' ';
     // }
      //fout << endl;
    //  fout << "flex status:" << '\t';
    //  fout << "reduce valid: " << '\t' << flex.flex_sim_gb_layer_reduce_is_valid << '\t';
    //  fout << "grouping num: " << '\t' << flex.flex_sim_gb_layer_reduce_grouping_num << '\n' << endl;
      wait(10, SC_NS);
    }

    std::ifstream fin;
    //fin.open("./flexnlp_results/flex_lstm_non_cluster.txt", ios::in);
    //fin.open("./flexnlp_results/flex_lstm_result.txt", ios::in);
    fin.open("./flexnlp_results/flex_lstm_4t_result.txt", ios::in);
    std::stringstream fs;

    std::string flex_str;
    std::string addr_str, data_str, data_byte_str;
    int addr_int, total, err, err_total, passed;
    total = err = err_total = passed = 0;

    while(std::getline(fin, addr_str, ',')) {
        err = 0;
        fs << "comparing addr @ " << addr_str << '\t';
        addr_str = addr_str.substr(5,5);
        addr_int = stoi(addr_str, nullptr, 16);
        std::getline(fin, data_str, '\n');

        for (int j = 0; j < 16; j++) {
            int index_ila = addr_int + 15 - j;
            int data_ila = flex.flex_sim_gb_core_large_buffer[index_ila].to_int();
            int data_flex = stoi(data_str.substr(2+2*j, 2), nullptr, 16);
            if (data_ila == data_flex)
                passed++;
            else
                err++;

            total++;
        }

        if (err>0) {
            fs << "ERROR!" << '\n';
            fs << "flex data: " << data_str << '\n';
            fs << "ila2 data : ";
            for (int j = 0; j < 16; j++) {
                fs << hex << flex.flex_sim_gb_core_large_buffer[addr_int + 15 - j] << " ";
            }
            fs << '\n';
        } else {
            fs << "PASSED!" << '\n';
        }
        err_total += err;
    }

    fout << "Comparison results" << '\n';
    if (total == passed)
        fout << "*********** TESTBENCH PASSED !!! **********" << '\n';
    else
        fout << "*********** TESTBENCH FAILED ... **********" << '\n';
    fout << dec << "#total: " << total << '\t';
    fout << "#passed: " << passed << '\t';
    fout << "#failed: " << err_total << '\n';

    fout << fs.rdbuf();

    fout.close();

    wait(10000, SC_NS);
    std::cout << "@" << sc_time_stamp() << " *********     sc_stop      *********" << std::endl;
//    std::fstream instr_log;
//    instr_log.open("./instr_out_flex.txt", ofstream::out | ofstream::trunc);
//    instr_log << flex.instr_log.rdbuf();
    flex.instr_log.close();
    sc_stop();
  }
};

int sc_main(int argc, char *argv[]) {
  cout << "test started" << endl;
  testbench tb("tb");
  sc_start();
  return 0;
}
