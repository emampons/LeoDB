#!/bin/bash
# Helper script to compile and run LeoDB

if [[ "$1" == "1" ]]
then
  # Delete data inside bin
  printf "Deleting /bin/data\n"
  rm -r bin/data
  exit 0
else
  # Else delete and re-create the data/ folder\
  printf "Deleting /data\n"
  rm -r data/
fi