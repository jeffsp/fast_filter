#include "main.h"
#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

void test4 ()
{
    const size_t M = 1024;
    const size_t N = 1024;
    const size_t K = 13;
    vector<double> x (M * N);

    // draw random values from a normal distribution
    random_device d;
    default_random_engine e (d ());
    std::normal_distribution<> n (0, 1);
    auto f = [&] { return n (e); };
    generate (begin (x), end (x), f);

    vector<double> y (M * N);

    // compute rms contrast slow
    auto start_time = chrono::high_resolution_clock::now ();
    filter<rms_contrast<double>> (x, y, M, N, K);
    auto end_time = chrono::high_resolution_clock::now ();
    auto t1 = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count ();
    cout << "t1: " << t1 << "ms" << endl;

    // compute rms contrast fast
    start_time = chrono::high_resolution_clock::now ();
    fast_filter<rms_contrast<double>> (x, y, M, N, K);
    end_time = chrono::high_resolution_clock::now ();
    auto t2 = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count ();
    cout << "t2: " << t2 << "ms" << endl;
    cout << static_cast<double> (t1) / t2 << "X speedup" << endl;
}

void process ()
{
    test4 ();
}
