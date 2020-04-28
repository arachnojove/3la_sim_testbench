#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

int main() {
    ifstream fin;
    fin.open("./flexnlp_sim_output_maxp.txt", ios::in);
    string data_line;
    stringstream fs;
    // cout << fin.is_open() << endl;
    getline(fin, data_line);
    // cout << data_line << endl;
    // cout << data_line.find("read") << endl;
    // cout << string::npos << endl;


    while (getline(fin, data_line)) {

        if (data_line.find("read") == string::npos) {
            // cout << "test" << endl;
            continue;
        }
        if (data_line.find("addr") != string::npos) {
            int addr_pos = data_line.find("0x");
            cout << "addr_pos" << addr_pos << endl;
            fs << data_line.substr(addr_pos, 10) << ',';
            continue;
        }
        if (data_line.find("data") != string::npos) {
            int data_pos_lo = data_line.find("0x");
            int data_pos_hi = data_line.rfind("resp");
            cout << "data_pos" << '\t' << data_pos_lo << '\t' << data_pos_hi;
            int data_len = data_pos_hi - data_pos_lo - 1;
            cout << '\t' << data_len << endl;
            fs << "0x";
            if (data_len < 34) {
                for (int i = data_len; i < 34; i++) 
                    fs << '0';
            }
            if (data_len > 34) {
                data_line.erase(data_pos_lo + 2, 1);
            }
            fs << data_line.substr(data_pos_lo+2, data_len-2);
            fs << '\n';
            continue;
        }

    }

    ofstream fout;
    fout.open("./flex_result.txt", ofstream::out | ofstream::trunc);
    fout << fs.rdbuf();
    fout.close();

    return 0;
}