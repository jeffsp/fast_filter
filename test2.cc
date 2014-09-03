#include "main.h"
#include <vector>

template<typename T,typename U>
void test2 ()
{
    const size_t M = 13;
    const size_t N = 23;
    const size_t K = 2;
    vector<T> x (M * N, 2);
    for (size_t i = 0; i < x.size (); i += 2)
        x[i] = 1;
    vector<U> y (M * N);
    filter<average<T>> (x, y, M, N, K);
    return;
    for (size_t i = K / 2; i + K / 2 < M; ++i)
        for (size_t j = K / 2; j + K / 2 < N; ++j)
            if (is_integral<U>::value)
                verify (x[index (i, j, N)] == 2);
            else
                verify (x[index (i, j, N)] == 1.5);
}

void process ()
{
    test2<unsigned char, unsigned char> ();
    test2<char, float> ();
    test2<double, short> ();
    test2<float, double> ();
}
