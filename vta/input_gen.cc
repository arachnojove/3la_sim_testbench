#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

void init_mem_instr(vector<json>& cmd_v, int num, int mode) {
  int addr_base = 0;
  switch (mode) {
    case 0: addr_base = 0; break;
    case 1: addr_base = 0; break;
    case 3: addr_base = 0x800; break;
    default: addr_base = 0;
  }
  for (int i = 0; i < num; i++) {
    json cmd;
    string mem_data = "0" + std::to_string(i);
    cmd = {{"instr_in", "0x0"},
           {"mem_mode", mode},
           {"mem_addr", addr_base + i},
           {"mem_uop_in", mem_data},
           {"mem_inp_in", mem_data},
           {"mem_wgt_in", mem_data},
           {"mem_bias_in", mem_data}};
    cmd_v.push_back(cmd);
  }
}

int main(int argc, char* argv[]) {
  string file_in;
  string file_out;

  file_in = argv[1];
  file_out = argv[2];

  ifstream fin;
  fin.open(file_in, ios::in);

  vector<json> cmd_v;
  init_mem_instr(cmd_v, 1024/4, 3);
  
  string instr_rd;
  while(getline(fin, instr_rd)) {
    json cmd;
    string instr_wr = "0x0" + instr_rd;
    cmd = {{"instr_in", instr_wr},
           {"mem_mode", 0},
           {"mem_addr", 0},
           {"mem_uop_in", "0"},
           {"mem_inp_in", "0"},
           {"mem_wgt_in", "0"},
           {"mem_bias_in", "0"}};
    cmd_v.push_back(cmd);
  }

  json json_out;
  json_out["program fragment"] = cmd_v;
  
  ofstream fout;
  fout.open(file_out, ios::out|ios::trunc);
  fout << setw(2) << json_out;
  fout.close();
  fin.close();
  
}
