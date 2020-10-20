#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>

#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

int main(int argc, char* argv[]) {
  string file_in;
  string file_out;

  file_in = "./testbench_input/file_list";

  ifstream file_list_fin;
  file_list_fin.open(file_in, ios::in);

  string log_file_name;
  while(getline(file_list_fin, log_file_name)) {
    cout << "start extracting info from: " << log_file_name << endl;
    ifstream log_file_in;
    ofstream input_data_out;
    // ofstream output_data_out;

    log_file_in.open("./testbench_input/" + log_file_name, ios::in);
    vector<json> cmd_v;

    input_data_out.open("./prog_frag/" + log_file_name + "_input.json", ios::out|ios::trunc);
    // output_data_out.open("./vivado_test_output/"+log_file_name+"_out.json", ios::out|ios::trunc);

    int instr_cntr = 0;
    string temp, mode, addr, data;

    while(getline(log_file_in, temp, ',')) {
      getline(log_file_in, mode, ',');
      getline(log_file_in, addr, ',');
      getline(log_file_in, data, '\n');

      json cmd;
      cmd = { {"_instr_No.", instr_cntr},
              {"mode", mode},
              {"addr", addr},
              {"data", data} };
      cmd_v.push_back(cmd);
      instr_cntr++;
    }
    json cmd_out;
    cmd_out["program fragment"] = cmd_v;
    input_data_out << setw(2) << cmd_out;

    log_file_in.close();
    input_data_out.close();
    cout << "finish extraction" << endl;
  }

  file_list_fin.close();
  return 0;
}