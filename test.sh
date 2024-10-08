#! /usr/bin/bash

INPUTS=$(ls tests/e1 | grep entrada_)
mkdir output/

for file in $INPUTS; do
  cat tests/e1/$file >  output/output_$file.txt
  echo '\n' >> output/output_$file.txt
  cat tests/e1/$file | ./etapa1 >> output/output_$file.txt
  # Your code to process each file
done
