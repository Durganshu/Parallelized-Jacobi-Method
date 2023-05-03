#!/bin/bash

# Assign the filename
filename="Makefile"
# Declare an array of string with type
declare -a StringArray=("CFLAGS = -O1 -qopt-report-annotate -qopt-report-phase=vec,loop" "CFLAGS = -O2 -qopt-report-annotate -qopt-report-phase=vec,loop" "CFLAGS = -O3 -qopt-report-annotate -qopt-report-phase=vec,loop" "CFLAGS = -Ofast -qopt-report-annotate -qopt-report-phase=vec,loop"
                        "CFLAGS = -O2 -xhost -qopt-report-annotate -qopt-report-phase=vec,loop" "CFLAGS = -O2 -xCORE-AVX512 -qopt-zmm-usage=high -qopt-report-annotate -qopt-report-phase=vec,loop" "CFLAGS = -O2 -xCORE-AVX512 -qopt-zmm-usage=high -ipo -qopt-report-annotate -qopt-report-phase=vec,loop" "CFLAGS = -O2 -ipo -qopt-report-annotate -qopt-report-phase=vec,loop"
                        "CFLAGS = -O2 -xCORE-AVX512 -fno-alias -qopt-zmm-usage=high -qopt-report-annotate -qopt-report-phase=vec,loop" "CFLAGS = -O2 -xCORE-AVX512 -fno-alias -qopt-zmm-usage=high -qopt-report-annotate -qopt-report-phase=vec,loop" "CFLAGS = -O2 -fno-alias -qopt-report-annotate -qopt-report-phase=vec,loop"
                        "CFLAGS = -O3 -xhost -qopt-report-annotate -qopt-report-phase=vec,loop" "CFLAGS = -O3 -xCORE-AVX512 -qopt-report-annotate -qopt-report-phase=vec,loop" "CFLAGS = -O3 -xCORE-AVX512 -ipo -qopt-zmm-usage=high -qopt-report-annotate -qopt-report-phase=vec,loop" "CFLAGS = -O3 -ipo -qopt-zmm-usage=high -qopt-report-annotate -qopt-report-phase=vec,loop"
                        "CFLAGS = -O3 -xCORE-AVX512 -fno-alias -qopt-zmm-usage=high -qopt-report-annotate -qopt-report-phase=vec,loop" "CFLAGS = -O3 -xCORE-AVX512 -fno-alias -qopt-report-annotate -qopt-report-phase=vec,loop" "CFLAGS = -O3 -fno-alias -qopt-report-annotate -qopt-report-phase=vec,loop"
                        "CFLAGS = -Ofast -xhost -qopt-report-annotate -qopt-report-phase=vec,loop" "CFLAGS = -Ofast -xCORE-AVX512 -qopt-zmm-usage=high -qopt-report-annotate -qopt-report-phase=vec,loop" "CFLAGS = -Ofast -xCORE-AVX512 -ipo -qopt-zmm-usage=high -qopt-report-annotate -qopt-report-phase=vec,loop" "CFLAGS = -Ofast -ipo -qopt-report-annotate -qopt-report-phase=vec,loop"
                        "CFLAGS = -Ofast -xCORE-AVX512 -fno-alias -qopt-zmm-usage=high -qopt-report-annotate -qopt-report-phase=vec,loop" "CFLAGS = -Ofast -xCORE-AVX512 -fno-alias -qopt-zmm-usage=high -qopt-report-annotate -qopt-report-phase=vec,loop" "CFLAGS = -Ofast -fno-alias -qopt-report-annotate -qopt-report-phase=vec,loop")
searchString="CFLAGS = -O0"
cp makefileinit $filename
for ((i =0; i < ${#StringArray[@]}; i++)) do
# echo ${StringArray[$i]}
 echo "Cleaning files"
 make clean
 sleep 3
 sed -i "s/$searchString/${StringArray[$i]}/" $filename
 echo -e "Replacement done\n"
 sleep 3
 echo "Building new code"
 make
 mv "relax_jacobi.c.annot" "relax_jacobi_$i.c.annot"
 sleep 10
 echo "Running the executable"
 echo -e "Compiler flags used: ${StringArray[$i]}" >> log.txt
 sbatch job.scp
 sleep 90
 echo -e "---------------------------------------------" >> log.txt
 searchString=${StringArray[$i]}
done
