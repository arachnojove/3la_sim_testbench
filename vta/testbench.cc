#include <systemc>
#include <iostream>
#include <sstream>
#include <fstream>

#include "nlohmann/json.hpp"
#include "vta.h"

using json = nlohmann::json;

std::string prog_frag_path;
std::string ref_out_path;

SC_MODULE(Source) {
  sc_in<bool> clk{"clk"};

  sc_out < sc_biguint<128> > vta_instr_in;
  sc_out < sc_biguint<3> > vta_mem_mode_in;
  sc_out < sc_biguint<32> > vta_mem_addr_in;
  sc_out < sc_biguint<32> > vta_mem_uop_data_in;
  sc_out < sc_biguint<8> > vta_mem_inp_data_in;
  sc_out < sc_biguint<8> > vta_mem_wgt_data_in;
  sc_out < sc_biguint<32> > vta_mem_bias_data_in;
  
  sc_out< sc_biguint<1> > input_done;

  SC_CTOR(Source) {
    SC_THREAD(source_input);
    sensitive << clk.pos();
  }

  void source_input() {
    // reset the port
    vta_instr_in = 0;
    vta_mem_mode_in = 0;
    vta_mem_addr_in = 0;
    vta_mem_uop_data_in = 0;
    vta_mem_inp_data_in = 0;
    vta_mem_wgt_data_in = 0;
    vta_mem_bias_data_in = 0;

    input_done = 0;
    wait(100, SC_NS);

    // read program fragment from file
    std::ifstream fin;
    fin.open(prog_frag_path);
    // fin.open("prog_frag.json", ios::in);
    // fin.open("./prog_frag/ALU_test_of_add imm-batch=16-vector_size=128-uop_compression=0_input.json");
    // fin.open("./prog_frag/ALU_test_of_add imm-batch=16-vector_size=128-uop_compression=1_input.json");
    // fin.open("./prog_frag/ALU_test_of_add-batch=16-vector_size=128-uop_compression=0_input.json");
    // fin.open("./prog_frag/ALU_test_of_add-batch=16-vector_size=128-uop_compression=1_input.json");
    // fin.open("./prog_frag/ALU_test_of_max imm-batch=16-vector_size=128-uop_compression=0_input.json");
    // fin.open("./prog_frag/ALU_test_of_max imm-batch=16-vector_size=128-uop_compression=1_input.json");
    // fin.open("./prog_frag/Blocked_GEMM_test-batch=16-channels=16-block=16-uop_comp=0-vt=1_input.json");
    // fin.open("./prog_frag/Blocked_GEMM_test-batch=256-channels=256-block=64-uop_comp=0-vt=1_input.json");
    // fin.open("./prog_frag/GEMM_test-batch=1-in_channels=16-out_channels=16-uop_comp=0_input.json");
    // fin.open("./prog_frag/GEMM_test-batch=4-in_channels=64-out_channels=64-uop_comp=0_input.json");
    //parse the json file
    json cmd_seq;
    fin >> cmd_seq;

    // pass the command to the ports
    for (int i = 0; i < cmd_seq["program fragment"].size(); i++) {
      std::string instr = cmd_seq["program fragment"][i]["instr_in"].get<std::string>();
      vta_instr_in = instr.c_str();
      vta_mem_mode_in = cmd_seq["program fragment"][i]["mem_mode"].get<int>();
      vta_mem_addr_in = cmd_seq["program fragment"][i]["mem_addr"].get<int>();
      vta_mem_uop_data_in = 
        (cmd_seq["program fragment"][i]["mem_uop_in"].get<std::string>()).c_str();
      vta_mem_inp_data_in = 
        (cmd_seq["program fragment"][i]["mem_inp_in"].get<std::string>()).c_str();
      vta_mem_wgt_data_in = 
        (cmd_seq["program fragment"][i]["mem_wgt_in"].get<std::string>()).c_str();
      vta_mem_bias_data_in = 
        (cmd_seq["program fragment"][i]["mem_bias_in"].get<std::string>()).c_str();

      wait(10, SC_NS);
    }

    input_done = 1;
  }

};

SC_MODULE(testbench) {
  vta vta_inst;
  Source src;

  sc_clock clk;
  sc_signal< sc_biguint<128> > vta_instr_in_signal;
  sc_signal< sc_biguint<3> > vta_mem_mode_in_signal;
  sc_signal< sc_biguint<32> > vta_mem_addr_in_signal;
  sc_signal< sc_biguint<32> > vta_mem_uop_data_in_signal;
  sc_signal< sc_biguint<8> > vta_mem_inp_data_in_signal;
  sc_signal< sc_biguint<8> > vta_mem_wgt_data_in_signal;
  sc_signal< sc_biguint<32> > vta_mem_bias_data_in_signal;

  sc_signal< sc_biguint<1> > input_done;

  SC_CTOR(testbench) :
    clk("clk", 1, SC_NS),
    vta_inst("vta_inst"),
    src("source")
  {
    src.clk(clk);
    src.vta_instr_in(vta_instr_in_signal);
    src.vta_mem_mode_in(vta_mem_mode_in_signal);
    src.vta_mem_addr_in(vta_mem_addr_in_signal);
    src.vta_mem_uop_data_in(vta_mem_uop_data_in_signal);
    src.vta_mem_inp_data_in(vta_mem_inp_data_in_signal);
    src.vta_mem_wgt_data_in(vta_mem_wgt_data_in_signal);
    src.vta_mem_bias_data_in(vta_mem_bias_data_in_signal);

    src.input_done(input_done);
  

    vta_inst.vta_vta_top_instr_in_in(vta_instr_in_signal);
    vta_inst.vta_vta_vir_mem_mode_in_in(vta_mem_mode_in_signal);
    vta_inst.vta_vta_vir_mem_addr_in_in(vta_mem_addr_in_signal);
    vta_inst.vta_vta_vir_mem_uop_data_in_in(vta_mem_uop_data_in_signal);
    vta_inst.vta_vta_vir_mem_inp_data_in_in(vta_mem_inp_data_in_signal);
    vta_inst.vta_vta_vir_mem_wgt_data_in_in(vta_mem_wgt_data_in_signal);
    vta_inst.vta_vta_vir_mem_bias_data_in_in(vta_mem_bias_data_in_signal);

    SC_THREAD(run);
  }

  void run() {
    vta_inst.instr_log.open("instr_log.txt", ios::out | ios::trunc);

    std::cout << "start running" << std::endl;
    std::cout << "*********** simulation start ***********" << std::endl;
    wait(10, SC_NS);

    while (input_done == 0) {
      std::cout << "current simulation time: " << '\t' << sc_time_stamp() << "\r" << std::flush;
      wait(10, SC_NS);
    }

    wait(1000, SC_NS);
    std::ifstream ref_out_file;
    ref_out_file.open(ref_out_path);
    // ref_out_file.open("./vivado_test_output/ALU_test_of_add imm-batch=16-vector_size=128-uop_compression=0_out.json");
    // ref_out_file.open("./vivado_test_output/ALU_test_of_add imm-batch=16-vector_size=128-uop_compression=1_out.json");
    // ref_out_file.open("./vivado_test_output/ALU_test_of_add-batch=16-vector_size=128-uop_compression=0_out.json");
    // ref_out_file.open("./vivado_test_output/ALU_test_of_add-batch=16-vector_size=128-uop_compression=1_out.json");
    // ref_out_file.open("./vivado_test_output/ALU_test_of_max imm-batch=16-vector_size=128-uop_compression=0_out.json");
    // ref_out_file.open("./vivado_test_output/ALU_test_of_max imm-batch=16-vector_size=128-uop_compression=1_out.json");
    // ref_out_file.open("./vivado_test_output/Blocked_GEMM_test-batch=16-channels=16-block=16-uop_comp=0-vt=1_out.json");
    // ref_out_file.open("./vivado_test_output/Blocked_GEMM_test-batch=256-channels=256-block=64-uop_comp=0-vt=1_out.json");
    // ref_out_file.open("./vivado_test_output/GEMM_test-batch=1-in_channels=16-out_channels=16-uop_comp=0_out.json");
    // ref_out_file.open("./vivado_test_output/GEMM_test-batch=4-in_channels=64-out_channels=64-uop_comp=0_out.json");

    json ref_out_json;
    ref_out_file >> ref_out_json;
    std::cout << '\n' << std::endl;

    int cnt = 0;
    int err = 0;
    for (int i = 0; i < ref_out_json["sim result"].size(); i++) {
      int out_addr = ref_out_json["sim result"][i]["output_addr"].get<int>();
      sc_biguint<8> ref_data = 
        (ref_out_json["sim result"][i]["output_data"].get<std::string>()).c_str();
      if (vta_inst.vta_vta_virtual_dram_out[out_addr] != ref_data) {
        err++;
        std::cout << std::dec << "error found at output mem address: " << out_addr << '\t';
        std::cout << std::hex << "ref_data: " << ref_data << '\t';
        std::cout << std::hex << "model_data: " << vta_inst.vta_vta_virtual_dram_out[out_addr] << std::endl;
      }
    }
    if (err == 0)
      std::cout << "testbench pass!" << std::endl;

    wait(100000, SC_NS);
    std::cout << '\n' << std::endl;
    std::cout << "************* sc_stop **************" << std::endl;

    vta_inst.instr_log.close();
    sc_stop(); 
  }
};

int sc_main(int argc, char* argv[]) {

  std::string file_name;
  file_name = argv[1];
  prog_frag_path = "./prog_frag/" + file_name + "_input.json";
  ref_out_path = "./vivado_test_output/" + file_name + "_out.json";
  std::cout << prog_frag_path << '\t' << ref_out_path << std::endl;
  std::cout << "test start for " << file_name << std::endl;
  testbench tb("tb");
  sc_start();

  // std::cout << "test start" << std::endl;
  // testbench tb("tb");
  // sc_start();

  return 0;
}
