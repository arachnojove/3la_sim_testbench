# VTA-ila simulation validation testbench
This file contains testbenches for simulation validation of vta-ila against VTA vivado-hls implementation.

## Related repo
- [vta-ila](https://github.com/LeeOHzzZ/vta-ila)
- [VTA HLS Design](https://github.com/apache/incubator-tvm-vta/tree/master/hardware/xilinx)

## testbench workflow
- Run VTA HLS simulation with provided test cases/test libs, and dump simulation log (see vivado_hls.log)
- Extract vta-ila systemc simulation needed info from vivado_hls.log, generate program fragments and reference output
- Run vta-ila systemc simulation testbench with given test case and compare the simulation result with reference output to check correctness

## Folder Organization
### tool
This folder contains tools for extracting information from vivado_hls.log
### vivado_test_log
This folder contains ILAtor simulation for each given testcase, extracted from vivado_hls.log
### vivado_test_output
This folder contains reference output data from VTA HLS simulation
### prog_frag
This folder contains program fragments input to ILAtor simulation for each test case

## Build and run testbench
- You can use following command to run all the testbench
````bash
./run_all.sh
````
- If you want to run one specific testbench 
````bash
# if you haven't generated the program fragments and reference output
make input_gen extract_sim_log
./extract_sim_log.out
./input_gen.out
````
You can pick one test case from ./vivado_test_log/file_list, and add it to the argument of the testbench executable, for example:
````bash
make testbench.out
./testbench.out ALU_test_of_min-imm-batch=16-vector_size=128-uop_compression=1
````

