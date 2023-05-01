#!/bin/bash

# Assign the filename
filename="Makefile"
# Declare an array of string with type
declare -a StringArray=("CFLAGS=-O1" "CFLAGS=-O2"
                   "CFLAGS=-O3" "CFLAGS=-Ofast" )
searchString="CFLAGS = -O0"
for val in ${StringArray[@]}; do

 echo "Cleaning files"
 make clean
 sleep 3
 sed -i "s/$searchString/$val/" $filename
 echo -e "Replacement done\n"
 sleep 3
 echo "Building new code"
 make
 sleep 10
 echo "Running the executable"
 echo -e "Compiler flags used: $val" >> log.txt
 for i in {1..2}; do 
    echo "run $i" >> log.txt 
    ./heat test.dat >> log.txt
 done
 echo -e "---------------------------------------------" >> log.txt
 searchString=$val
done
