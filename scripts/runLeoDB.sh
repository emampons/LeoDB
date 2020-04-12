#!/bin/bash
# Helper script to compile and run LeoDB

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

# Assert needed folder exists
mkdir -p bin
mkdir -p data

# Compile LeoDB
echo "Compiling LeoDB...  "

# Check if tests need to be compiled
if [[ "$RUN_TESTS" == 1  ]]; then
    printf "Compiling and Running Tests...  \n"
    LEODB_DIRECTORY=$(pwd)
    echo ${LEODB_DIRECTORY}
    ansible-playbook scripts/runTestSuit.yml -e path_to_leoDB=${LEODB_DIRECTORY}
else
  printf "Compiling and Running Without Tests...  \n"
  cmake . -DBUILD_TESTS=0
  make
  printf "\nRunning leoDB...  \n"
  ./bin/leodb
fi

# (Optional) Clean up
if [[ "$CLEAN_UP" == 1  ]]; then
  printf "\nCleaning Up...\n"
  scripts/cleanUp.sh
fi
