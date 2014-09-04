#include "main.h"
#include <algorithm>
#include <random>
#include <vector>

template<typename T>
void test4 (double mean, double variance)
{
    const size_t M = 16;
    const size_t N = 16;
    const size_t K = 3;
    vector<T> x (M * N);

    // draw random values from a normal distribution
    default_random_engine e;
    std::normal_distribution<> n (mean, variance);
    auto f = [&] { return n (e); };
    generate (begin (x), end (x), f);

    // compute average
    vector<T> y1 = slow_average (x, M, N, K);
    vector<T> y2 = fast_average (x, M, N, K);
    //clog << "x" << endl;
    //print (clog, x, M, N);
    //clog << "y1" << endl;
    //print (clog, y1, M, N);
    //clog << "y2" << endl;
    //print (clog, y2, M, N);
    verify (y1 == y2);

    // compute variance
    y1 = slow_variance (x, M, N, K);
    y2 = fast_variance (x, M, N, K);
    verify (y1 == y2);

    // compute stddev
    y1 = slow_stddev (x, M, N, K);
    y2 = fast_stddev (x, M, N, K);
    verify (y1 == y2);

    // compute rms contrast
    y1 = slow_rms_contrast (x, M, N, K);
    y2 = fast_rms_contrast (x, M, N, K);
    verify (y1 == y2);
}

void process ()
{
    test4<char> (0, 10);
    test4<unsigned char> (100, 10);
    test4<short> (0, 10);
    test4<unsigned short> (100, 10);
    test4<int> (0, 10);
    test4<unsigned int> (100, 10);
    test4<long> (0, 10);
    test4<unsigned long> (100, 10);
}
