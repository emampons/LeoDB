//
// Created by Sid Premkumar on 4/12/20.
//
#include <benchmark/benchmark.h>
#include <db.cpp>

static void test_put_get_500(benchmark::State& state) {
    /*
     * Test putting 500 <int, int> into LeoDB and then getting the first value put
     */
    int rand_array[500];
    for(int i=1;i<=500;i++){
        rand_array[i]=(std::rand() % 1000000) + 1;
    }
    DB<int, int> leodb("any_level", "data", "/leodb-log.log");
    for (int x = 0; x < 500; x++){
        leodb.put(rand_array[x], rand_array[x]);
    }

    while (state.KeepRunning()) {
        int ret = leodb.get(rand_array[0]).getItem();
        assert(ret == rand_array[0]);
    }
    leodb.close();
}

// Register the function as a benchmark
BENCHMARK(test_put_get_500)->Unit(benchmark::kMillisecond);;

// Run the benchmark
BENCHMARK_MAIN();


