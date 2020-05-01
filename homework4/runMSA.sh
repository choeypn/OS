##Bash script
##Nat Choeypant

#!bin/bash

numIterations=${1}

seq1=${2}
seq2=${3}
seq3=${4}
seq4=${5}
seq5=${6}

seqLength=${#seq1}

trialNum=0
largestOutputYet=0

for numTries in `seq 1 ${numIterations}`; do 
  
  output=$(java randomMSA ${seq1} ${seq2} ${seq3} ${seq4} ${seq5})

  trialNum=$((trialNum + 1))

  if [ $output -gt $largestOutputYet ]; then
      largestOutputYet=$output
      echo " Score: $output on try number $numTries"
      while read line; do
          echo $line
      done < "msa.txt"
  fi

done

