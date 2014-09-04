#include "main.h"
#include <algorithm>
#include <random>
#include <vector>

template<typename T>
void test3 ()
{
    const size_t M = 64;
    const size_t N = 64;
    const size_t K = 31;
    vector<T> x (M * N);

    // draw random values from a normal distribution
    default_random_engine e;
    std::normal_distribution<> n (0, 1);
    auto f = [&] { return n (e); };
    generate (begin (x), end (x), f);

    auto mm = minmax_element (begin (x), end (x));
    verify (*mm.first < 0.0);
    verify (*mm.second > 0.0);

    // compute average
    vector<T> y = slow_average (x, M, N, K);
    mm = minmax_element (begin (y), end (y));
    verify (*mm.first > -1.0);
    verify (*mm.second < 1.0);

    // compute variance
    y = slow_variance (x, M, N, K);
    mm = minmax_element (begin (y), end (y));
    verify (*mm.first >= 0.0);
    verify (*mm.second < 2.0);

    // compute stddev
    y = slow_stddev (x, M, N, K);
    mm = minmax_element (begin (y), end (y));
    verify (*mm.first >= 0.0);
    verify (*mm.second < 1.1);
    verify (*mm.second > 0.9);

    // compute rms contrast
    y = slow_rms_contrast (x, M, N, K);
    mm = minmax_element (begin (y), end (y));
    verify (*mm.first >= 0.0);
    verify (*mm.second < 1.1);
    verify (*mm.second > 0.9);
}

void process ()
{
    test3<double> ();
}
