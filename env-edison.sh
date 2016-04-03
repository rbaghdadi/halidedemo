#!/bin/bash

export XTPE_LINK_TYPE=dynamic
export CRAYPE_LINK_TYPE=dynamic
export MPICXX=CC

module swap PrgEnv-intel PrgEnv-gnu
module unload atp
module unload cray-shmem
module unload xt-shmem
module load zlib
module load python
module load numpy
module load tmux
module load git

export LLVM_CONFIG=$HOME/llvm3.5-edison/Release+Asserts/bin/llvm-config
export CLANG=$HOME/llvm3.5-edison/Release+Asserts/bin/clang
