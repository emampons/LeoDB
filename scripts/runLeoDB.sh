#!/bin/bash

# Args
RUN_TESTS=$1
CLEAN_UP=$2

# Assert Args
if ! [[ "$RUN_TESTS" =~ ^[0-1]+$ ]]; then
    echo "ERROR! Arg 1 (RUN_TESTS) must be 0 or 1!"
    exit 0
fi

if ! [[ "$CLEAN_UP" =~ ^[0-1]+$ ]]; then
    echo "ERROR! Arg 2 (CLEAN_UP) must be 0 or 1!"
    exit 0
fi

# Compile LeoDB
echo "Compiling LeoDB...  "

# Check if tests need to be compiled
if [[ "$RUN_TESTS" == 1  ]]; then
    echo "Compiling and Running Tests...  \n"
    cmake . -DBUILD_TESTS=1
    make
    scripts/runTests.sh
else
  echo "Compiling and Running Without Tests...  \n"
  cmake . -DBUILD_TESTS=0
  make
  printf "\nRunning leoDB...  \n"
  ./leodb
fi

# (Optional) Clean up
if [[ "$CLEAN_UP" == 1  ]]; then
  printf "\nCleaning Up...\n"
  scripts/cleanUp.sh
fi





## Fuctions
#runLeoDB () {
#  printf "OUTPUT: \n"
#  ./leodb
#  printf "\n"
#}
#
## Delete state if prompted
#if [ $2 = "1" ]; then
#  printf "DELETING state...\n"
#  log_path="data/leodb-log.log"
#  rm $log_path
#  printf "Deleted log \n"
#  manifest_path="data/manifest.leodb"
#  rm $manifest_path
#  printf "Deleted manifest \n"
#  rm data/in-memory.leodb
#  printf "Deleted in-memory.leodb \n"
#  rm -R -- data/*/
#  printf "Deleted all level info \n"
#else
#  printf "NOT deleting state...\n"
#fi
#
#
#if [ $1 = "1" ]; then
#  # Check if we recently compiled leodb
#   printf "Using recently created file... \n"
#   runLeoDB
#   exit 1
#elif [ $1 = "2" ]; then
#  # Check if we want to quckly compile and run
#   printf "Running make and running... \n"
#   make
#   runLeoDB
#   exit 1
#else:
#  # Else invalid!
#  printf "Invalid first argument"
#  exit 1
#fi
#
#printf "Running run.yml..."
#if ansible-playbook run.yml -e clean_up=0; then
#  prinf "Compiled successfully, running LeoDB..."
#  runLeoDB
#else
#  prinf "Error compiling"
#  exit 1
#fi
