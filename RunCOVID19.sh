#!/bin/bash
# start sampler with
# -l ODEmodel11S26P4U.so    shared library of model
Model=covid19
ModelXML=./vfgen/${Model}.vf
ModelSO=./C/${Model}.so
ModelHDF5=./data/${Model}.h5
SampleFileDate="`date +%Y-%m-%dT%Hh%Mm`"
SampleFile="$SampleFileDate.h5"

DefaultSampleSize=100000
DefaultWarmUp=2000
SampleSize=${2:-$DefaultSampleSize}
WarmUp=${3:-$DefaultWarmUp}
StepSize="0.02";
T0="-40"
M="1.5"

#RESUME="TRUE"

MCMC_SIZE="-w ${WarmUp} -s ${SampleSize}"
MCMC_PARAMETERS="-i ${StepSize} -m $M ${RESUME:+--resume} -p"
PARALLEL_TEMPERING="-g 1"
ODE_PARAMETERS="--init-at-t ${T0} --max-step 7000 --rel-tol 1e-6 --abs-tol 1e-13"
MCMC_FILES="-o ${SampleFile} -l ${ModelSO} --hdf5 ${ModelHDF5}"
OPTIONS="${MCMC_SIZE} ${MCMC_FILES} ${MCMC_PARAMETERS} ${PARALLEL_TEMPERING} ${ODE_PARAMETERS}"
n_cpu=`egrep processor /proc/cpuinfo | wc -l`
((n_cores=n_cpu/2))
NP=${1:-$n_cores}
P=`h5dump -A -d /prior/mu ${ModelHDF5} | egrep DATASPACE | sed -E 's/.*\{\s*\( ([0-9]+).*/\1/'`


export OMP_NUM_THREADS=2

cat<<EOF
$0
 Number of ODE model parameters: $P
 t₀=$T0

 output will be one .h5 file per MPI proc,
 ending with: …${SampleFile}

      MPI workers: ${NP}
   OpenMP Threads: ${OMP_NUM_THREADS:-automatic}

 to load the sample in GNU Octave 
 use the load() function

 sampling now ...
EOF

MPICMD=mpirun

SMMALA="$HOME/mcmc_clib/bin/ode_smmala"

# the actual parameter estimation:
ModelHDF5="./data/${Model}.h5"
SampleFile="all_data_$SampleFileDate"
CurrentCommand="$MPICMD -np $NP $SMMALA $OPTIONS"
echo ${CurrentCommand} 
${CurrentCommand} 2> ${Model}.err
