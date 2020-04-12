# Scripts

## What is this?
This folder contains all the scripts needed to run and compile LeoDB and associated tests.

## How can I use this?
You can use the scripts as normal bash scripts.

* `cleanUp.sh`
  * This script cleans up our working directory (used after `cmake` and `make` to clean the repo of untracked files)
* `deleteData.sh`
  * This script deletes our `/data` folder for our DB
  * Run `deleteData.sh 1` to delete `/bin/data`
* `runLeoDB.sh`
  * This is the main script used to run LeoDB
  * There are two arguments (passed in order):
    * `$1`: If set to 1, will compile and run tests through ansible-playbooks
    * `$2`: Clean up environment (set to 1 to call `cleanUp.sh` after running leodb)

## Tests
We have two types of testing available. We have unit tests and benchmark tests. Learn more about them below.

### Requirements
You will need to have installed [Ansible](https://docs.ansible.com/ansible/latest/installation_guide/intro_installation.html) to run our test suit.

### Benchmarking
Benchmarking tests are done through [Googles Benchmarking Module](https://github.com/google/benchmark).
We store all our benchmarking code under [tests/benchmark](../tests/benchmark).
You can run the benchmarking tests one of two ways:

1. You can manually compile the tests and run them individually

  a. `cmake . -DBUILD_TESTS=1 && make`

  b. Then run individual tests by name: `./test_put_500`

  c. The information about the test will then be printed on the console

2. (**Prefered**) You can run our benchmarking test suit using [Ansible](https://docs.ansible.com/ansible/latest/index.html)

  a. `ansible-playbook scripts/runBenchmarkSuit.yml -e path_to_leoDB=$PATH_TO_LEODB`

    * Where `$PATH_TO_LEODB` is the path to the root of LeoDB (i.e. `user/sidpremkumar/LeoDB`)


### Unit Testing
Coming soon...
