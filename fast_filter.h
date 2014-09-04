/// @file fast_filter.h
/// @brief fast filter operations
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2014-09-02

#pragma once

#include <cassert>
#include <cmath>
#include <iostream>
#include <type_traits>
#include <vector>

namespace fast_filter_ops
{
    size_t index (const size_t i, const size_t j, const size_t cols)
    {
        return i * cols + j;
    }

    class average
    {
        private:
        size_t total;
        double sum;

        public:
        average ()
            : total (0)
            , sum (0)
        {
        }
        template<typename T>
        void update (const T &x)
        {
            ++total;
            sum += x;
        }
        double result () const
        {
            return sum / total;
        }
    };

    class variance
    {
        private:
        size_t total;
        double sum;
        double sum2;

        public:
        variance ()
            : total (0)
            , sum (0)
            , sum2 (0)
        {
        }
        template<typename T>
        void update (const T &x)
        {
            ++total;
            sum += x;
            sum2 += (x * x);
        }
        double result () const
        {
            const double mean = sum / total;
            return sum2 / total - mean * mean;
        }
    };

    class stddev
    {
        private:
        variance v;

        public:
        template<typename T>
        void update (const T &x)
        {
            v.update (x);
        }
        double result ()
        {
            return sqrt (v.result ());
        }
    };

    class rms_contrast : public stddev { };

    template<typename OP,typename T>
    T slow_filter (const T &p, const size_t rows, const size_t cols, const size_t krows, const size_t kcols)
    {
        T q (p.size ());
        for (size_t i = 0; i + krows < rows + 1; ++i)
        {
            for (size_t j = 0; j + kcols < cols + 1; ++j)
            {
                OP op;
                for (size_t i2 = 0; i2 < krows; ++i2)
                {
                    for (size_t j2 = 0; j2 < kcols; ++j2)
                    {
                        const size_t n = index (i + i2, j + j2, cols);
                        assert (n < p.size ());
                        op.update (p[n]);
                    }
                }
                const size_t n = index (i + krows / 2, j + kcols / 2, cols);
                assert (n < q.size ());
                const double y = op.result ();
                if (std::is_integral<typename T::value_type>::value)
                    q[n] = ::round (y);
                else
                    q[n] = y;
            }
        }
        return q;
    }

    template<typename OP,typename T>
    T slow_filter (const T &p, const size_t rows, const size_t cols, const size_t k)
    {
        return slow_filter<OP,T> (p, rows, cols, k, k);
    }

    template<typename T>
    T slow_average (const T &p, const size_t rows, const size_t cols, const size_t krows, const size_t kcols)
    {
        return slow_filter<average> (p, rows, cols, krows, kcols);
    }

    template<typename T>
    T slow_average (const T &p, const size_t rows, const size_t cols, const size_t k)
    {
        return slow_average (p, rows, cols, k, k);
    }

    template<typename T>
    T slow_variance (const T &p, const size_t rows, const size_t cols, const size_t krows, const size_t kcols)
    {
        return slow_filter<variance> (p, rows, cols, krows, kcols);
    }

    template<typename T>
    T slow_variance (const T &p, const size_t rows, const size_t cols, const size_t k)
    {
        return slow_variance (p, rows, cols, k, k);
    }

    template<typename T>
    T slow_stddev (const T &p, const size_t rows, const size_t cols, const size_t krows, const size_t kcols)
    {
        return slow_filter<stddev> (p, rows, cols, krows, kcols);
    }

    template<typename T>
    T slow_stddev (const T &p, const size_t rows, const size_t cols, const size_t k)
    {
        return slow_stddev (p, rows, cols, k, k);
    }

    template<typename T>
    T slow_rms_contrast (const T &p, const size_t rows, const size_t cols, const size_t krows, const size_t kcols)
    {
        return slow_filter<rms_contrast> (p, rows, cols, krows, kcols);
    }

    template<typename T>
    T slow_rms_contrast (const T &p, const size_t rows, const size_t cols, const size_t k)
    {
        return slow_rms_contrast (p, rows, cols, k, k);
    }

    template<typename T,typename U>
    void block_sum (const size_t row, const T &p, U &q, const size_t cols, const size_t kcols)
    {
        double sum = 0;
        for (size_t j = 0; j < cols; ++j)
        {
            const size_t n1 = index (row, j, cols);
            assert (n1 < p.size ());
            sum += p[n1];
            // make sure we have enough in the sum
            if (j + 1 < kcols)
                continue;
            // write to q's transposed location
            const size_t n2 = index (j - kcols / 2, row, cols);
            assert (n2 < q.size ());
            q[n2] = sum;
            // remove old pixel value
            const size_t n3 = index (row, j - kcols + 1, cols);
            assert (n3 < p.size ());
            sum -= p[n3];
        }
    }

    template<typename T>
    T fast_average (const T &p, const size_t rows, const size_t cols, const size_t krows, const size_t kcols)
    {
        std::vector<double> tmp1 (p.size ());

        // pass 1: block sum p's rows and write to tmp
        for (size_t i = 0; i < rows; ++i)
            block_sum (i, p, tmp1, cols, kcols);

        std::vector<double> tmp2 (p.size ());

        // pass 2: block sum tmp's rows and write to q
        for (size_t j = 0; j < cols; ++j)
            block_sum (j, tmp1, tmp2, rows, krows);

        T q (p);

        // pass 3: compute average
        for (size_t i = 0; i < q.size (); ++i)
        {
            const double y = tmp2[i] / (krows * kcols);
            if (std::is_integral<typename T::value_type>::value)
                q[i] = ::round (y);
            else
                q[i] = y;
        }

        return q;
    }

    template<typename T>
    T fast_average (const T &p, const size_t rows, const size_t cols, const size_t k)
    {
        return fast_average (p, rows, cols, k, k);
    }

    template<typename T>
    T fast_variance (const T &p, const size_t rows, const size_t cols, const size_t krows, const size_t kcols)
    {
        return p;
    }

    template<typename T>
    T fast_variance (const T &p, const size_t rows, const size_t cols, const size_t k)
    {
        return fast_variance (p, rows, cols, k, k);
    }

    template<typename T>
    T fast_stddev (const T &p, const size_t rows, const size_t cols, const size_t krows, const size_t kcols)
    {
        return p;
    }

    template<typename T>
    T fast_stddev (const T &p, const size_t rows, const size_t cols, const size_t k)
    {
        return fast_stddev (p, rows, cols, k, k);
    }

    template<typename T>
    T fast_rms_contrast (const T &p, const size_t rows, const size_t cols, const size_t krows, const size_t kcols)
    {
        return p;
    }

    template<typename T>
    T fast_rms_contrast (const T &p, const size_t rows, const size_t cols, const size_t k)
    {
        return fast_rms_contrast (p, rows, cols, k, k);
    }
}
