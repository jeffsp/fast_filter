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
void test3 ()
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

    //clog << "input\t"; print_min_max (clog, x);
    auto mm = minmax_element (begin (x), end (x));
    verify (*mm.first < 0.0);
    verify (*mm.second > 0.0);

    // compute average
    vector<U> y (M * N);
    filter<average<T>> (x, y, M, N, K);
    //clog << "avg\t"; print_min_max (clog, y);
    mm = minmax_element (begin (y), end (y));
    verify (*mm.first > -1.0);
    verify (*mm.second < 1.0);

    // compute variance
    filter<variance<T>> (x, y, M, N, K);
    //clog << "var\t"; print_min_max (clog, y);
    mm = minmax_element (begin (y), end (y));
    verify (*mm.first >= 0.0);
    verify (*mm.second < 2.0);

    // compute stddev
    filter<stddev<T>> (x, y, M, N, K);
    //clog << "sd\t"; print_min_max (clog, y);
    mm = minmax_element (begin (y), end (y));
    verify (*mm.first >= 0.0);
    verify (*mm.second < 1.1);
    verify (*mm.second > 0.9);

    // compute rms contrast
    filter<rms_contrast<T>> (x, y, M, N, K);
    //clog << "rmse\t"; print_min_max (clog, y);
    mm = minmax_element (begin (y), end (y));
    verify (*mm.first >= 0.0);
    verify (*mm.second < 1.1);
    verify (*mm.second > 0.9);
}

void process ()
{
    test3<double, double> ();
}
