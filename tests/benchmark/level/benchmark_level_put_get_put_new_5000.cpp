//
// Created by Sid Premkumar on 3/23/20.
//
#include <benchmark/benchmark.h>
#include <db.cpp>
#include <assert.h>

static void test_put_get_put_new_5000(benchmark::State& state) {
    /*
     * Test putting 2500 <int, int> into LeoDB, reading 10% of values 5x (i.e. 500), put 2500 more, then read 10% of values again
     */
    int rand_array[5000];
    for(int i=1;i<=5000;i++){
        rand_array[i]=(std::rand() % 1000000) + 1;
    }

    DB<int, int> leodb("level", "data", "/leodb-log.log");
    for (auto _ : state) {
        for (int x = 0; x < 2500; x++){
            leodb.put(rand_array[x], rand_array[x]);
        }

        for (int i = 0; i < 5; i++ ) {
            for (int x = 2000; x < 2500; x++) {
                // Get and assert our result is correct
                int ret = leodb.get(rand_array[x]).getItem();
                assert(ret == rand_array[x]);
            }
        }

        for (int x = 2500; x < 5000; x++){
            leodb.put(rand_array[x], rand_array[x]);
        }

        for (int i = 0; i < 5; i++ ) {
            for (int x = 1500; x < 2000; x++) {
                // Get and assert our result is correct
                int ret = leodb.get(rand_array[x]).getItem();
                assert(ret == rand_array[x]);
            }
        }
    }

    // Close our DB
    leodb.close();
}

// Register the function as a benchmark
BENCHMARK(test_put_get_put_new_5000)->Unit(benchmark::kMillisecond);;

// Run the benchmark
BENCHMARK_MAIN();

