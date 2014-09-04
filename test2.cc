#include "main.h"
#include <vector>

// Verify that the naive implementation of average() works
template<typename T>
void test2 ()
{
    const size_t M = 13;
    const size_t N = 23;
    const size_t K = 2;
    vector<T> x (M * N, 2);
    for (size_t i = 0; i < x.size (); i += 2)
        x[i] = 1;
    vector<T> y = slow_average (x, M, N, K);
    for (size_t i = K / 2; i + K / 2 < M; ++i)
        for (size_t j = K / 2; j + K / 2 < N; ++j)
            if (is_integral<T>::value)
                verify (y[index (i, j, N)] == 2);
            else
                verify (y[index (i, j, N)] == 1.5);
}

void process ()
{
    test2<char> ();
    test2<unsigned char> ();
    test2<short> ();
    test2<unsigned short> ();
    test2<int> ();
    test2<unsigned int> ();
    test2<long> ();
    test2<unsigned long> ();
    test2<float> ();
    test2<double> ();
}
