#!/bin/bash

args=("$@")
inputFile="${args[0]}"
input_dir="${args[1]}"
numFilesPerDirectory="${args[2]}"

#Check arguments
if [ ! -f "$inputFile" ]; then

    printf "The file '%s' does not exist\n" $inputFile
    exit 1
fi

if (( $numFilesPerDirectory < 1 )); then

    printf "Only positive numbers > 0 are allowed\n"
    exit 1
fi

#Check if directory already exists.
if [ -d "$input_dir" ]; then

    printf "Directory '%s' already exists. Exit...\n" $input_dir
    exit 1
else

    mkdir $input_dir
fi

#Read and store records from file inputFile
index=0
while read line ; do
    records[$index]="$line"
    index=$(($index+1))
done < <(grep "" $inputFile)

#Store only the countries
countries=( $(awk '{print $4}' inputFile) )
#Store only the unique countries
countries=( `for i in ${countries[@]}; do echo $i; done | sort -u` )

#Create all sub-directories with files 'Country-X.txt'
for i in "${countries[@]}"; do

    mkdir "${input_dir}/$i"

    for ((j = 0 ; j < $numFilesPerDirectory ; j++)); do
        
        touch "${input_dir}/${i}/${i}-$((j+1)).txt"
    done
done


declare -a citizen_entry
declare -a pointer_country
new_line=$'\n'


#Each index of array represents a file with information for citizens
#so for each index of array search the corresponding country and store
#all the records for this file.
for ((i = 0 ; i < $index ; i++)); do

    country=$(echo "${records[$i]}" | awk '{print $4}')

    for j in "${!countries[@]}"; do

        if [[ "${countries[$j]}" = "${country}" ]]; then

            #'pointer_country' is the counter that shows in which file we are at round-robin scheduling.
            pos=$(( pointer_country[$j] % $numFilesPerDirectory ))
            (( pointer_country[$j]++ ))

            #find index of array for current country
            current_index=$(($j * $numFilesPerDirectory))

            str="${records[$i]}$new_line"
            citizen_entry[$(( $current_index+$pos ))]+=$str

            break
        fi
    done

done


#Write the records of each index of array at the corresponding file
for ((i = 0 ; i < ${#countries[@]} ; i++)); do

    for ((j = 0 ; j < $numFilesPerDirectory ; j++)); do

        file="${input_dir}/${countries[i]}/${countries[i]}-$((j+1)).txt"
        printf "%s\n" "${citizen_entry[$(( $i * $numFilesPerDirectory + $j ))]}" > $file
        
        truncate -s -1 $file
    done
done


exit 0