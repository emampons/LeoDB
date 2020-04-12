//
// Created by Sid Premkumar on 4/12/20.
//
#include <benchmark/benchmark.h>
#include <db.cpp>

static void test_put_1000(benchmark::State& state) {
    /*
     * Test putting 500 <int, int> into LeoDB
     */
    for (auto _ : state) {
        DB<int, int> leodb;
        for (int x = 0; x < 1000; x++){
            leodb.put(x, x);
        }
        leodb.close();
    }
}

// Register the function as a benchmark
BENCHMARK(test_put_1000)->Unit(benchmark::kMillisecond);;

// Run the benchmark
BENCHMARK_MAIN();


