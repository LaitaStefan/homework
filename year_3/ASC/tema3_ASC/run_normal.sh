#! /bin/bash


for f in $(ls /export/asc/btc_input/); do
	for i in 1 2 4 8; do
		./ppu/tema3 0 $i "/export/asc/btc_input/$f" "$f.out0.btc" "$f.out0.pgm"
	done
done