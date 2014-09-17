#! /bin/bash

qsub -cwd -q ibm-cell-qs22.q -pe openmpi*12 12 run.sh