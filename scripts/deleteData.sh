#!/bin/bash

# Verbose, show commands as we run them
set -x

# Helper script to compile and run LeoDB

if [ -z "$1" ]
  then
    # Delete data inside bin
    printf "Deleting /scripts/data\n"
    rm -r scripts/data
    exit 0
fi

# Else delete and re-create the data/ folder\
printf "Deleting /data\n"
rm -r data/
