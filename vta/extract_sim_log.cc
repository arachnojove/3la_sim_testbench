#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

int main() {

  ifstream fin;
  fin.open("vivado_hls.log", ios::in);
  ofstream file_name_out;
  file_name_out.open("./vivado_test_log/file_list", ios::out|ios::trunc);

  string temp;
  while(getline(fin, temp)) {
    if (temp.find("[INSTR-START]") == string::npos) 
      continue;
    auto tmp_pos = temp.find("]");
    string cur_file_name = temp.substr(tmp_pos+4);
    file_name_out << cur_file_name << "\n";

    ofstream out_log;
    out_log.open("vivado_test_log/" + cur_file_name, ios::out|ios::trunc);

    while (getline(fin, temp)) {
      if (temp.find("test successful!") != string::npos)
        break;
      if (temp.find("[ILA-INFO]") == string::npos)
        continue;
      tmp_pos = temp.find("]");
      out_log << temp.substr(tmp_pos+2) << "\n";
    }
    out_log.close();
  }
  file_name_out.close();
  
  return 0;
}
