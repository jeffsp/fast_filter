#include "main.h"
#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

// Get some timing results
void test5 ()
{
    const size_t M = 2048;
    const size_t N = 2048;
    const size_t K = 31;
    vector<int> x (M * N);

    // draw random values from a normal distribution
    default_random_engine e;
    std::normal_distribution<> n (0, 10);
    auto f = [&] { return n (e); };
    generate (begin (x), end (x), f);

    // compute rms contrast slow
    auto start_time = chrono::high_resolution_clock::now ();
    vector<int> y = slow_average (x, M, N, K);
    auto end_time = chrono::high_resolution_clock::now ();
    auto t1 = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count ();
    cout << "t1: " << t1 << "ms" << endl;

    // compute rms contrast fast
    start_time = chrono::high_resolution_clock::now ();
    y = fast_average (x, M, N, K);
    end_time = chrono::high_resolution_clock::now ();
    auto t2 = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count ();
    cout << "t2: " << t2 << "ms" << endl;
    cout << static_cast<double> (t1) / t2 << "X speedup" << endl;
}

void process ()
{
    test5 ();
}
