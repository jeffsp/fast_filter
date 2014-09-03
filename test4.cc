#include "main.h"
#include <algorithm>
#include <random>
#include <vector>

template<typename T>
void print_min_max (ostream &s, const T &x)
{
    auto mm = minmax_element (begin (x), end (x));
    s << "min: " << *mm.first << '\t';
    s << "max: " << *mm.second << endl;
}

template<typename T,typename U>
void test4 ()
{
    const size_t M = 64;
    const size_t N = 64;
    const size_t K = 31;
    vector<T> x (M * N);

    // draw random values from a normal distribution
    random_device d;
    default_random_engine e (d ());
    std::normal_distribution<> n (0, 1);
    auto f = [&] { return n (e); };
    generate (begin (x), end (x), f);

    auto mm = minmax_element (begin (x), end (x));
    verify (*mm.first < 0.0);
    verify (*mm.second > 0.0);

    // compute average
    vector<U> y1 (M * N);
    vector<U> y2 (M * N);
    filter<average<T>> (x, y1, M, N, K);
    fast_filter<fast_average<T>> (x, y2, M, N, K);
    verify (y1 == y2);

    // compute variance
    filter<variance<T>> (x, y1, M, N, K);
    fast_filter<fast_variance<T>> (x, y2, M, N, K);
    verify (y1 == y2);

    // compute stddev
    filter<stddev<T>> (x, y1, M, N, K);
    fast_filter<fast_stddev<T>> (x, y2, M, N, K);
    verify (y1 == y2);

    // compute rms contrast
    filter<rms_contrast<T>> (x, y1, M, N, K);
    fast_filter<fast_rms_contrast<T>> (x, y2, M, N, K);
    verify (y1 == y2);
}

void process ()
{
    test4<double, double> ();
}
