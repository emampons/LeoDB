# LeoDB Source Code
Following is a list of class's along with the functions they implement

## Overview of LeoDB
Following is a high level idea of how data is flushed to memory in LeoDB:

1. Once we have exceeded our `MEMORY_THRESHOLD` we call the function `INIT_WRITE_TO_FILE` to start writing to disk, inside that function we:
  * Sort our in-memory data
  * Call the `flush_new_level` starting with our root level and our sorted data
2. Inside `flush_new_level` we check if we can add another run into the current level:
  * If we *can*:
    * Build a output string with all of our data (i.e. `hash::key:::value::::tombstone`)
    * Call the function `add_data_to_level` with our output string and level_info
  * If we *cannot*:
    * We found a full level!
    * Gather data on the current level (and delete that content on disk)
    * Sort the gathered data with our in-memory data
    * Recursively call `flush_new_level` with our new sorted data
3. Inside `add_data_to_level` we:
  * Consult our monitor class to determine what type of level we should output (**TODO**)
  * Dump our data to disk

