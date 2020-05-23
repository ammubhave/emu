#!/bin/sh
 
full_path=$(realpath $0)
dir_path=$(dirname $full_path)

echo "Generating $dir_path/cpu/insts/AllInstructions.h"
$dir_path/py/genAllInstructions.py > $dir_path/cpu/insts/AllInstructions.h
echo "Generating $dir_path/cpu/InstructionDecoder.cpp"
$dir_path/py/genInstructionDecoder.py
