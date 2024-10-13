#! /usr/bin/bash

INPUTS=$(ls tests/e2 | grep entrada_)
mkdir -p output/

for file in $INPUTS; do
  cat tests/e2/$file >  output/output_$file.txt
  echo '\n' >> output/output_$file.txt
  if ! cat tests/e2/$file | ./etapa2 >> output/output_$file.txt 2>> output/output_$file.txt; then
    echo "An error occurred while processing $file with etapa2." >> output/output_$file.txt
  fi
done
