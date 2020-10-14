rm -rf ./vivado_test_log/* ./vivado_test_output/* ./prog_frag/*
make extract_sim_log input_gen
./extract_sim_log.out
./input_gen.out
make testbench.out

file_list="./vivado_test_log/file_list"
while IFS= read -r line
do
    ./testbench.out $line
done <"$file_list"
