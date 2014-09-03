#include "main.h"
#include <vector>

template<typename T,typename U>
void test1 ()
{
    const size_t M = 17;
    const size_t N = 13;
    const size_t K1 = 5;
    const size_t K2 = 7;
    vector<T> x (M * N, 100);
    vector<U> y (M * N);
    filter<average<T>> (x, y, M, N, K1, K2);
    for (size_t i = K1 / 2; i + K1 / 2 < M; ++i)
        for (size_t j = K2 / 2; j + K2 / 2 < N; ++j)
            verify (x[index (i, j, N)] == y[index (i, j, N)]);
}

void process ()
{
    test1<unsigned char, unsigned char> ();
    test1<char, float> ();
    test1<double, short> ();
}
