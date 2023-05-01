#!/bin/bash

# Assign the filename
filename="Makefile"
# Declare an array of string with type
declare -a StringArray=("CFLAGS=-O1" "CFLAGS=-O2"
                   "CFLAGS=-O3" "CFLAGS=-Ofast" )
searchString="CFLAGS = -O0"
for val in ${StringArray[@]}; do
 #echo -e $val
 #sed -i "/CFLAGS =/c $var" $filename
 #sed -i 's/maxmemory.*/maxmemory 26gb/'
 make clean
 sleep 3
 sed -i "s/$searchString/$val/" $filename
 sleep 3
 echo -e "Replacement done\n"
 make
 sleep 10
 #echo -e "Replacement done\n"
 searchString=$val
 #cat $filename
done

# if [[ $search != "" && $replace != "" ]]; then
#   sed -i "s/$search/$replace/" $filename
# fi