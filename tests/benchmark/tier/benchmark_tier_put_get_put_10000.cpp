//
// Created by Sid Premkumar on 3/23/20.
//
#include <benchmark/benchmark.h>
#include <db.cpp>
#include <assert.h>

static void test_put_get_put_10000(benchmark::State& state) {
    /*
     * Test putting 5000 <int, int> into LeoDB, reading 10% of values 5x (i.e. 1000), put 5000 more, then read 10% of values again
     */
    DB<int, int> leodb("tier", "data", "/leodb-log.log");
    for (auto _ : state) {
        for (int x = 0; x < 5000; x++){
            leodb.put(x, x);
        }

        for (int i = 0; i < 5; i++ ) {
            for (int x = 4000; x < 5000; x++) {
                // Get and assert our result is correct
                int ret = leodb.get(x).getItem();
                assert(ret == x);
            }
        }

        for (int x = 5000; x < 10000; x++){
            leodb.put(x, x);
        }

        for (int i = 0; i < 5; i++ ) {
            for (int x = 4000; x < 5000; x++) {
                // Get and assert our result is correct
                int ret = leodb.get(x).getItem();
                assert(ret == x);
            }
        }
    }

    // Close our DB
    leodb.close();
}

// Register the function as a benchmark
BENCHMARK(test_put_get_put_10000)->Unit(benchmark::kMillisecond);;

// Run the benchmark
BENCHMARK_MAIN();

