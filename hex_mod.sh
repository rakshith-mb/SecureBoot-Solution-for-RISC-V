#!/bin/bash

# Script to convert elf file to hex file to enable edits

cp test test.elf
riscv32-unknown-elf-objcopy -O ihex test.elf myfile.hex
echo "Done!"