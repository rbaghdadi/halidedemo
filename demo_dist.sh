#make -B distributed

# Run the distributed job

export MV2_ENABLE_AFFINITY=0
export MPICH_MAX_THREAD_SAFETY=multiple
export HL_JIT_TARGET=host
export CRAYPE_LINK_TYPE=dynamic

# Run with 24 threads per node.
srun -p debug -t 00:00:30 -N 128 -n 128 -c 24 ./distributed 500000 500000

# UNCOMMENT DURING DEMO TIME ONLY:
# srun -p regular -t 00:01:00 -N 128 -n 128 -c 24 -A m2534 --reservation=my_reservation_name ./distributed 500000 500000

