#!/bin/bash

# Papa Florin, 334CA
# Tema 2 ASC

[[ -z $COMPILER ]] && COMPILER="gcc"

if [[ $COMPILER="gcc" ]]; then
   	./basic 35000
   	./dtrmv_n 35000
   	./improved 35000
fi
