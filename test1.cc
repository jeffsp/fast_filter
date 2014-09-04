#include "main.h"
#include <vector>

// Verify that the naive implementation of average() works
template<typename T>
void test1 ()
{
    const size_t M = 17;
    const size_t N = 13;
    const size_t K1 = 5;
    const size_t K2 = 7;
    vector<T> x (M * N, 100);
    vector<T> y = slow_average (x, M, N, K1, K2);
    for (size_t i = K1 / 2; i + K1 / 2 < M; ++i)
        for (size_t j = K2 / 2; j + K2 / 2 < N; ++j)
            verify (x[index (i, j, N)] == y[index (i, j, N)]);
}

void process ()
{
    test1<char> ();
    test1<unsigned char> ();
    test1<short> ();
    test1<unsigned short> ();
    test1<int> ();
    test1<unsigned int> ();
    test1<long> ();
    test1<unsigned long> ();
    test1<float> ();
    test1<double> ();
}
