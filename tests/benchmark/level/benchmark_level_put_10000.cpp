//
// Created by Sid Premkumar on 4/12/20.
//
#include <benchmark/benchmark.h>
#include <db.cpp>

static void test_put_10000(benchmark::State& state) {
    /*
     * Test putting 500 <int, int> into LeoDB
     */
    int rand_array[10000];
    for(int i=1;i<=10000;i++){
        rand_array[i]=(std::rand() % 1000000) + 1;
    }
    DB<int, int> leodb("level", "data", "/leodb-log.log");
    while (state.KeepRunning()) {
        for (int x = 0; x < 10000; x++){
            benchmark::DoNotOptimize(leodb.put(rand_array[x], rand_array[x]));
        }
    }
    leodb.close();
}

// Register the function as a benchmark
BENCHMARK(test_put_10000)->Unit(benchmark::kMillisecond);;

// Run the benchmark
BENCHMARK_MAIN();


