
# Script to convigure the PULP environment

module load riscv/riscv xilinx/2019.2 questasim/2019.4 gcc/7.5.0
cd $1
source env/pulpissimo.sh
if [[ $2 -eq 1 ]]
then
    cd pulp-sdk
    source configs/pulpissimo.sh
    source configs/fpgas/pulpissimo/genesys2.sh
    cd ../
fi