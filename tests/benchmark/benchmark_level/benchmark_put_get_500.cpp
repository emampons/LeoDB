//
// Created by Sid Premkumar on 3/23/20.
//
#include <benchmark/benchmark.h>
#include <db.cpp>
#include <assert.h>

static void test_put_get_500(benchmark::State& state) {
    /*
     * Test putting 500 <int, int> into LeoDB and then searching for a the first number added
     */
    DB<int, int> leodb;
    for (auto _ : state) {
        for (int x = 0; x < 500; x++){
            leodb.put(x, x);
        }
    }

    // Get and assert our result is correct
    int ret = leodb.get(0).getItem();
    assert(ret == 1);

    // Close our DB
    leodb.close();
}

// Register the function as a benchmark
BENCHMARK(test_put_get_500)->Unit(benchmark::kMillisecond);;

// Run the benchmark
BENCHMARK_MAIN();


