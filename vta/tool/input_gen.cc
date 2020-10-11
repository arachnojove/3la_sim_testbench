#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

// void init_mem_instr(vector<json>& cmd_v, int num, int mode) {
//   int addr_base = 0;
//   switch (mode) {
//     case 0: addr_base = 0; break;
//     case 1: addr_base = 0; break;
//     case 3: addr_base = 0x800; break;
//     default: addr_base = 0;
//   }
//   for (int i = 0; i < num; i++) {
//     json cmd;
//     string mem_data = "0" + std::to_string(i);
//     cmd = {{"instr_in", "0x0"},
//            {"mem_mode", mode},
//            {"mem_addr", addr_base + i},
//            {"mem_uop_in", mem_data},
//            {"mem_inp_in", mem_data},
//            {"mem_wgt_in", mem_data},
//            {"mem_bias_in", mem_data}};
//     cmd_v.push_back(cmd);
//   }
// }

int main(int argc, char* argv[]) {
  string file_in;
  string file_out;

  // file_in = argv[1];
  // file_out = argv[2];

  file_in = "./vivado_test_log/file_list";

  ifstream file_list_fin;
  file_list_fin.open(file_in, ios::in);

  string log_file_name;
  while(getline(file_list_fin, log_file_name)) {
    cout << "start extracting info from: " << log_file_name << endl;
    ifstream log_file_in;
    ofstream input_data_out;
    ofstream output_data_out;

    log_file_in.open("./vivado_test_log/" + log_file_name, ios::in);
    vector<json> cmd_v;
    vector<json> result_v;
    string data_rd;

    input_data_out.open("./prog_frag/" + log_file_name + "_input.json", ios::out|ios::trunc);
    output_data_out.open("./vivado_test_output/"+log_file_name+"_out.json", ios::out|ios::trunc);

    int instr_cntr = 0;

    while(getline(log_file_in, data_rd)) {
      // get input data in
      if (data_rd.find("input_buffer") != string::npos) {
        string addr_in;
        addr_in = data_rd.substr(data_rd.find("[")+1, 
                                  data_rd.find("]") - data_rd.find("[") - 1);
        string data;
        data = data_rd.substr(data_rd.find(":")+2, 2);
        json cmd;
        cmd = { {"instr No.", instr_cntr},
                {"instr_in", "0x00"},
                {"mem_mode", 1},
                {"mem_addr", stoi(addr_in, nullptr, 10)},
                {"mem_uop_in", "0x" + data},
                {"mem_inp_in", "0x" + data},
                {"mem_wgt_in", "0x" + data},
                {"mem_bias_in", "0x" + data}};
        cmd_v.push_back(cmd);
        instr_cntr++;
      }
      // get weight data in 
      else if (data_rd.find("weight_buffer") != string::npos) {
        string addr_in;
        addr_in = data_rd.substr(data_rd.find("[")+1, 
                                  data_rd.find("]") - data_rd.find("[") - 1);
        string data;
        data = data_rd.substr(data_rd.find(":")+2, 2);
        json cmd;
        cmd = { {"instr No.", instr_cntr},
                {"instr_in", "0x00"},
                {"mem_mode", 2},
                {"mem_addr", stoi(addr_in, nullptr, 10)},
                {"mem_uop_in", "0x" + data},
                {"mem_inp_in", "0x" + data},
                {"mem_wgt_in", "0x" + data},
                {"mem_bias_in", "0x" + data}};
        cmd_v.push_back(cmd);
        instr_cntr++;
      }
      // get bias data in
      else if (data_rd.find("bias_buf") != string::npos) {
        string addr_in;
        addr_in = data_rd.substr(data_rd.find("[")+1, 
                                  data_rd.find("]") - data_rd.find("[") - 1);
        string data;
        data = data_rd.substr(data_rd.find(":")+2, 8);
        json cmd;
        cmd = { {"instr No.", instr_cntr},
                {"instr_in", "0x00"},
                {"mem_mode", 3},
                {"mem_addr", stoi(addr_in, nullptr, 10)},
                {"mem_uop_in", "0x" + data},
                {"mem_inp_in", "0x" + data},
                {"mem_wgt_in", "0x" + data},
                {"mem_bias_in", "0x" + data}};
        cmd_v.push_back(cmd);
        instr_cntr++;
      }
      // get uop
      else if (data_rd.find("uop") != string::npos) {
        string addr_in;
        addr_in = data_rd.substr(data_rd.find(".")+1,
                                  data_rd.find("info")-data_rd.find(".")-1);
        string data;
        data = data_rd.substr(data_rd.find(":")+2, 16);
        json cmd;
        cmd = { {"instr No.", instr_cntr},
                {"instr_in", "0x00"},
                {"mem_mode", 4},
                {"mem_addr", stoi(addr_in, nullptr, 10)},
                {"mem_uop_in", "0x" + data},
                {"mem_inp_in", "0x" + data},
                {"mem_wgt_in", "0x" + data},
                {"mem_bias_in", "0x" + data}};
        cmd_v.push_back(cmd);
        instr_cntr++;
      }
      // get instruction
      else if (data_rd.find("instr") != string::npos) {
        string ins = data_rd.substr(data_rd.find(":")+2, 32);
        json cmd;
        cmd = { {"instr No.", instr_cntr},
                {"instr_in", "0x0" + ins},
                {"mem_mode", 0},
                {"mem_addr", 0},
                {"mem_uop_in", "0"},
                {"mem_inp_in", "0"},
                {"mem_wgt_in", "0"},
                {"mem_bias_in", "0"}};
        cmd_v.push_back(cmd);
        instr_cntr++;
      }
      // get result
      else if (data_rd.find("out") != string::npos) {
        string addr;
        addr = data_rd.substr(data_rd.find("[")+1, 
                              data_rd.find("]") - data_rd.find("[") - 1);
        string data;
        data = data_rd.substr(data_rd.find(":") + 2, 8);
        json result;
        result = {{"output_addr", stoi(addr, nullptr, 10)},
                   {"output_data", "0x"+data}};
        result_v.push_back(result);
      }
    }

    json cmd_out;
    cmd_out["program fragment"] = cmd_v;
    json result_out; 
    result_out["sim result"] = result_v;

    input_data_out << setw(2) << cmd_out;
    output_data_out << setw(2) << result_out;

    log_file_in.close();
    input_data_out.close();
    output_data_out.close();

    cout << "finish extraction" << endl;
  }

  file_list_fin.close();
  return 0;  
}
