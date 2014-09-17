#!/bin/bash

# Papa Florin, 334CA
# Tema 2 ASC

rm -rf out
make clean
make

mprun.sh --job-name test_O --queue ibm-opteron.q \
	--modules "libraries/atlas-3.10.1-gcc-4.4.6-opteron" \
	--script exec_script_o.sh --show-qsub --show-script --batch-job 

mprun.sh --job-name test_N --queue ibm-nehalem.q \
	--modules "libraries/atlas-3.10.1-gcc-4.4.6-nehalem" \
	--script exec_script_o.sh --show-qsub --show-script --batch-job
 
mprun.sh --job-name test_q --queue ibm-quad.q \
	--modules "libraries/atlas-3.10.1-gcc-4.4.6-quad" \
	--script exec_script_o.sh --show-qsub --show-script --batch-job

echo "Waiting for output files to be created"

while [ ! -f ./out/test_O.o* ]; do
	sleep 1
done

while [ ! -f ./out/test_N.o* ]; do
	sleep 1
done

while [ ! -f ./out/test_q.o* ]; do
	sleep 1
done

echo "Output files created"
echo "Waiting for output files to be completed"

i=$(cat ./out/test_O.o* | wc -l)
j=$(cat ./out/test_N.o* | wc -l)
k=$(cat ./out/test_q.o* | wc -l)
while [ $i -lt 3 ] || [ $j -lt 3 ] || [ $k -lt 3 ]; do
	sleep 1
	i=$(cat ./out/test_O.o* | wc -l)
	j=$(cat ./out/test_N.o* | wc -l)
	k=$(cat ./out/test_q.o* | wc -l)
done

echo "Output complete"

echo "Opteron 23" > data1; cat ./out/test_O.o* | tr "\\n" " " >> data1; cat data1 | tr "\\n" " " > tdata; echo >> tdata
echo "Nehalem 23" > data1; cat ./out/test_N.o* | tr "\\n" " " >> data1; cat data1 | tr "\\n" " " >> tdata; echo >> tdata
echo "Quad 23" > data1; cat ./out/test_q.o* | tr "\\n" " " >> data1; cat data1 | tr "\\n" " " >> tdata
echo >> tdata
rm -f data1
make clean

gnuplot gnu.plot
