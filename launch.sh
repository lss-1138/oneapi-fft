#!/bin/bash
source /opt/intel/inteloneapi/setvars.sh > /dev/null 2>&1

mpirun -n 4 bin/fft_mpi.x 8

# 跑以下对比实验时，最好将FFT原数据和结果数据输出注释掉

# mpirun -n 1 bin/fft_mpi.x 1024
# mpirun -n 2 bin/fft_mpi.x 1024
# mpirun -n 4 bin/fft_mpi.x 1024
# mpirun -n 8 bin/fft_mpi.x 1024

# mpirun -n 1 bin/fft_mpi.x 2048
# mpirun -n 2 bin/fft_mpi.x 2048
# mpirun -n 4 bin/fft_mpi.x 2048
# mpirun -n 8 bin/fft_mpi.x 2048

# mpirun -n 1 bin/fft_mpi.x 4096
# mpirun -n 2 bin/fft_mpi.x 4096
# mpirun -n 4 bin/fft_mpi.x 4096
# mpirun -n 8 bin/fft_mpi.x 4096

# mpirun -n 1 bin/fft_mpi.x 8192
# mpirun -n 2 bin/fft_mpi.x 8192
# mpirun -n 4 bin/fft_mpi.x 8192
# mpirun -n 8 bin/fft_mpi.x 8192

# mpirun -n 1 bin/fft_mpi.x 16384
# mpirun -n 2 bin/fft_mpi.x 16384
# mpirun -n 4 bin/fft_mpi.x 16384
# mpirun -n 8 bin/fft_mpi.x 16384

# mpirun -n 1 bin/fft_mpi.x 32768
# mpirun -n 2 bin/fft_mpi.x 32768
# mpirun -n 4 bin/fft_mpi.x 32768
# mpirun -n 8 bin/fft_mpi.x 32768

# mpirun -n 1 bin/fft_mpi.x 65536
# mpirun -n 2 bin/fft_mpi.x 65536
# mpirun -n 4 bin/fft_mpi.x 65536
# mpirun -n 8 bin/fft_mpi.x 65536

# mpirun -n 1 bin/fft_mpi.x 131072
# mpirun -n 2 bin/fft_mpi.x 131072
# mpirun -n 4 bin/fft_mpi.x 131072
# mpirun -n 8 bin/fft_mpi.x 131072