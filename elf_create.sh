#!/bin/bash

# Script to convert modified hex back to elf file for the PULPissimo make framework to pick up

riscv32-unknown-elf-objcopy -I ihex -O elf32-little "myfile.hex" "mod.elf"
rm test
cp mod.elf test
cp test test_mod.elf
riscv32-unknown-elf-objcopy -O ihex test_mod.elf myfile_mod.hex
echo "Done ELF"