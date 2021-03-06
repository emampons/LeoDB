//
// Created by Sid Premkumar on 3/23/20.
//
#include <benchmark/benchmark.h>
#include <db.cpp>
#include <assert.h>

static void test_put_get_put_new_10000(benchmark::State& state) {
    /*
     * Test putting 10000 <int, int> into LeoDB, reading 10% of values 5x (i.e. 1000), put 10000 more, then read 10% of values again
     */
    int rand_array[10000];
    for(int i=1;i<=10000;i++){
        rand_array[i]=(std::rand() % 1000000) + 1;
    }

    DB<int, int> leodb("tier", "data", "/leodb-log.log");
    for (auto _ : state) {
        for (int x = 0; x < 5000; x++) {
            leodb.put(rand_array[x], rand_array[x]);
        }

        for (int i = 0; i < 5; i++ ) {
            for (int x = 4000; x < 5000; x++) {
                // Get and assert our result is correct
                int ret = leodb.get(rand_array[x]).getItem();
                assert(ret == rand_array[x]);
            }
        }

        for (int x = 5000; x < 10000; x++) {
            leodb.put(rand_array[x], rand_array[x]);
        }

        for (int i = 0; i < 5; i++ ) {\
            for (int x = 3000; x < 4000; x++) {
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
BENCHMARK(test_put_get_put_new_10000)->Unit(benchmark::kMillisecond);;

// Run the benchmark
BENCHMARK_MAIN();

