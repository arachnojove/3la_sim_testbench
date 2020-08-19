#include <nvhls_int.h>
#include <nvhls_types.h>
#include <nvhls_vector.h>
#include <nvhls_message.h>
#include <math.h> // pow()
#include <bitset> // std::bitset
#include "SM6Spec.h"
#include <TypeToBits.h>
#include "AdpfloatSpec.h"
#include "AdpfloatUtils.h"

#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_float.h>
#include <ac_math.h>

#include <systemc.h>
#include <fstream>
#include <sstream>
#include <string>

int sc_main(int argc, char* argv[]) {
    std::ifstream fin;
    std::string temp, mode, addr, input_cmd;

    fin.open("./testbench_input/axi_commands_for_2_lstm_64x64.txt", ios::in);
    //fin.open("./flexnlp_results/flex_lstm_non_cluster.txt", ios::in);

    ac_int<8, true> bias_w_ac;
    ac_int<8, true> bias_b_ac;
    ac_int<8, true> bias_i_ac;

    spec::AdpfloatBiasType bias_weight;
    spec::AdpfloatBiasType bias_bias;
    spec::AdpfloatBiasType bias_input;

    bias_input = 1;

    std::stringstream fs;

    //while(std::getline(fin, addr, ',')) {
    while(std::getline(fin, temp, ',')) {
        std::getline(fin, mode, ',');
        std::getline(fin, addr, ',');
        std::getline(fin, input_cmd, '\n');
        //std::cout << addr << std::endl;

        if (mode.compare("R") == 0)
            continue;
        if (addr.compare("0xdeaddead") == 0)
            continue;
        
        if(int(input_cmd[input_cmd.size()-1]) == 13) {
            input_cmd = input_cmd.substr(0, input_cmd.size()-1);
        }

        //std::cout << input_cmd << std::endl;
        std::string input_cmd_format;
        if (input_cmd.length() <= 34) {
            input_cmd_format.append(34 - input_cmd.length(), '0');
            input_cmd_format.append(input_cmd.substr(2));
        } else {
             input_cmd_format.append(input_cmd.substr(input_cmd.length()-32));
        }

        std::string local_addr = addr.substr(4);
        if (local_addr.compare("400020") == 0) {
            std::string bias_w_str = input_cmd.substr(input_cmd.size() - 4, 2);
            std::string bias_b_str = input_cmd.substr(input_cmd.size() - 6, 2);
            std::string bias_i_str = input_cmd.substr(input_cmd.size() - 8, 2);
            bias_w_ac = std::stoi(bias_w_str, nullptr, 16);
            bias_b_ac = std::stoi(bias_w_str, nullptr, 16);
            bias_i_ac = std::stoi(bias_i_str, nullptr, 16);

            std::cout << bias_w_str << '\t' << bias_b_str << '\t' << bias_i_str << std::endl;

            bias_weight = bias_w_ac;
            bias_bias = bias_b_ac;
            bias_input = bias_i_ac;

            std::cout << bias_weight << '\t' << bias_bias << '\t' << bias_input << std::endl;
            continue;
        }
        ac_int<32, false> addr_ac_int = std::stoi(addr, nullptr, 16);

        if (addr_ac_int >= 0x33500000 && addr_ac_int <= 0x335fffff) {
            fs << "GB: ";
            fs << "addr: " << std::hex << addr_ac_int << '\t' << "data: ";
            for (int i = 0; i < 16; i++) {
                std::string data_byte = input_cmd_format.substr(i*2,2);
                ac_int<8,false> data_byte_ac = std::stoi(data_byte, nullptr, 16);
                AdpfloatType<spec::kAdpfloatWordWidth, spec::kAdpfloatExpWidth> data_byte_af(data_byte_ac);
                float data_byte_float = data_byte_af.to_float(bias_input);
                fs << std::dec << data_byte_float << " ";
            }
            fs << std::endl << std::endl;
            continue;
        }
        // data for PE0
        if (addr_ac_int >= 0x34500000 && addr_ac_int <= 0x345fffff) {
            fs << "weight in PE0:";
            fs << "addr: " << std::hex << addr_ac_int << '\t' << "data: ";
            for (int i = 0; i < 16; i++) {
                std::string data_byte = input_cmd_format.substr(i*2,2);
                ac_int<8,false> data_byte_ac = std::stoi(data_byte, nullptr, 16);
                AdpfloatType<spec::kAdpfloatWordWidth, spec::kAdpfloatExpWidth> data_byte_af(data_byte_ac);
                float data_byte_float = data_byte_af.to_float(bias_weight);
                fs << std::dec << data_byte_float << " ";
            }
            fs << std::endl << std::endl;
            continue;
        }

        if (addr_ac_int >= 0x34600000 && addr_ac_int <= 0x346fffff) {
            fs << "bias in PE0" << std::endl;
            fs << "addr: " << std::hex << addr_ac_int << '\t' << "data: ";
            for (int i = 0; i < 16; i++) {
                std::string data_byte = input_cmd_format.substr(i*2,2);
                ac_int<8,false> data_byte_ac = std::stoi(data_byte, nullptr, 16);
                AdpfloatType<spec::kAdpfloatWordWidth, spec::kAdpfloatExpWidth> data_byte_af(data_byte_ac);
                float data_byte_float = data_byte_af.to_float(bias_bias);
                fs << std::dec << data_byte_float << " ";
            }
            fs << std::endl << std::endl;
            continue;
        }
        // data for PE1
        if (addr_ac_int >= 0x35500000 && addr_ac_int <= 0x355fffff) {
            fs << "weight in PE1" << std::endl;
            fs << "addr: " << std::hex << addr_ac_int << '\t' << "data: ";
            for (int i = 0; i < 16; i++) {
                std::string data_byte = input_cmd_format.substr(i*2,2);
                ac_int<8,false> data_byte_ac = std::stoi(data_byte, nullptr, 16);
                AdpfloatType<spec::kAdpfloatWordWidth, spec::kAdpfloatExpWidth> data_byte_af(data_byte_ac);
                float data_byte_float = data_byte_af.to_float(bias_weight);
                fs << std::dec << data_byte_float << " ";
            }
            fs << std::endl << std::endl;
            continue;
        }

        if (addr_ac_int >= 0x35600000 && addr_ac_int <= 0x356fffff) {
            fs << "bias in PE1" << std::endl;
            fs << "addr: " << std::hex << addr_ac_int << '\t' << "data: ";
            for (int i = 0; i < 16; i++) {
                std::string data_byte = input_cmd_format.substr(i*2,2);
                ac_int<8,false> data_byte_ac = std::stoi(data_byte, nullptr, 16);
                AdpfloatType<spec::kAdpfloatWordWidth, spec::kAdpfloatExpWidth> data_byte_af(data_byte_ac);
                float data_byte_float = data_byte_af.to_float(bias_bias);
                fs << std::dec << data_byte_float << " ";
            }
            fs << std::endl << std::endl;
            continue;
        }

         // data for PE2
        if (addr_ac_int >= 0x36500000 && addr_ac_int <= 0x365fffff) {
            fs << "weight in PE2" << std::endl;
            fs << "addr: " << std::hex << addr_ac_int << '\t' << "data: ";
            for (int i = 0; i < 16; i++) {
                std::string data_byte = input_cmd_format.substr(i*2,2);
                ac_int<8,false> data_byte_ac = std::stoi(data_byte, nullptr, 16);
                AdpfloatType<spec::kAdpfloatWordWidth, spec::kAdpfloatExpWidth> data_byte_af(data_byte_ac);
                float data_byte_float = data_byte_af.to_float(bias_weight);
                fs << std::dec << data_byte_float << " ";
            }
            fs << std::endl << std::endl;
            continue;
        }

        if (addr_ac_int >= 0x36600000 && addr_ac_int <= 0x366fffff) {
            fs << "bias in PE2" << std::endl;
            fs << "addr: " << std::hex << addr_ac_int << '\t' << "data: ";
            for (int i = 0; i < 16; i++) {
                std::string data_byte = input_cmd_format.substr(i*2,2);
                ac_int<8,false> data_byte_ac = std::stoi(data_byte, nullptr, 16);
                AdpfloatType<spec::kAdpfloatWordWidth, spec::kAdpfloatExpWidth> data_byte_af(data_byte_ac);
                float data_byte_float = data_byte_af.to_float(bias_bias);
                fs << std::dec << data_byte_float << " ";
            }
            fs << std::endl << std::endl;
            continue;
        }
        // data for PE3
        if (addr_ac_int >= 0x37500000 && addr_ac_int <= 0x375fffff) {
            fs << "weight in PE3" << std::endl;
            fs << "addr: " << std::hex << addr_ac_int << '\t' << "data: ";
            for (int i = 0; i < 16; i++) {
                std::string data_byte = input_cmd_format.substr(i*2,2);
                ac_int<8,false> data_byte_ac = std::stoi(data_byte, nullptr, 16);
                AdpfloatType<spec::kAdpfloatWordWidth, spec::kAdpfloatExpWidth> data_byte_af(data_byte_ac);
                float data_byte_float = data_byte_af.to_float(bias_weight);
                fs << std::dec << data_byte_float << " ";
            }
            fs << std::endl << std::endl;
            continue;
        }

        if (addr_ac_int >= 0x37600000 && addr_ac_int <= 0x376fffff) {
            fs << "bias in PE3" << std::endl;
            fs << "addr: " << std::hex << addr_ac_int << '\t' << "data: ";
            for (int i = 0; i < 16; i++) {
                std::string data_byte = input_cmd_format.substr(i*2,2);
                ac_int<8,false> data_byte_ac = std::stoi(data_byte, nullptr, 16);
                AdpfloatType<spec::kAdpfloatWordWidth, spec::kAdpfloatExpWidth> data_byte_af(data_byte_ac);
                float data_byte_float = data_byte_af.to_float(bias_bias);
                fs << std::dec << data_byte_float << " ";
            }
            fs << std::endl << std::endl;
            continue;
        }

    }
    std::fstream fout;
    fout.open("float_data_2_lstm_64x64.txt", ofstream::out | ofstream::trunc);
    fout << fs.rdbuf();
    fout.close();
    fin.close();
    
    return 0;
}
