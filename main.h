/// @file main.h
/// @brief main boilerplate
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2014-09-02

#include "fast_filter.h"
#include "utils.h"

using namespace std;
using namespace fast_filter_ops;

void process ();

template<typename T>
void print (ostream &s, const T &p, const size_t rows, const size_t cols)
{
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
            if (is_integral<typename T::value_type>::value)
                s << ' ' << int (p [index (i, j, cols)]);
            else
                s << ' ' << p [index (i, j, cols)];
        s << endl;
    }
}

int main (int , char **)
{
    try
    {
        process ();

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what () << std::endl;
        return -1;
    }
}
