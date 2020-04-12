# LeoDB
LeoDB is a database aimed at exploring how we can use Leveling and Tiering simultaneously in order to explore the possible performance benefits. 


## Setting Up Environment
The following are the things you need to do before running LeoDB:

1. Pull and Sync Git Submodules

    a. To do this run the following commands:
        ```
            git submodule init && git submodule sync
        ```
## Run LeoDB & Tests
```shell script
    ./scripts/runLeoDB $RUN_TESTS $CLEAN_UP
```
Where:
1. `$RUN_TEST`: Runs test (either 0/1)
2. `$CLEAN_UP`: Cleans up our environment (either 0/1)

Running LeoDB normally will execute anything inside `main.cpp`. Modify this according to what you want to use.

Learn more about the different scripts and tests [here](/scripts/README.md)