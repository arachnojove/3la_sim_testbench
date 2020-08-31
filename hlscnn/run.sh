export HLSCNN_DIR=/home/yl29/3LA/HLSCNN_Accel/cmod/harvard/top
cp $HLSCNN_DIR/test_conv.log ./sim_log/
make all
./extract_input_seq.out ./sim_log/test_conv.log ./test_input_conv.json
./conv_testbench.out

