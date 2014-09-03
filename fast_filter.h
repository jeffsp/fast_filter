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

namespace fast_filter_ops
{
    size_t index (const size_t i, const size_t j, const size_t cols)
    {
        return i * cols + j;
    }

    template<typename T> struct sum_traits { typedef T sum_type; };
    template<> struct sum_traits<char> { typedef long sum_type; };
    template<> struct sum_traits<short> { typedef long sum_type; };
    template<> struct sum_traits<int> { typedef long sum_type; };
    template<> struct sum_traits<unsigned char> { typedef unsigned long sum_type; };
    template<> struct sum_traits<unsigned short> { typedef unsigned long sum_type; };
    template<> struct sum_traits<unsigned int> { typedef unsigned long sum_type; };
    template<> struct sum_traits<float> { typedef double sum_type; };
    template<> struct sum_traits<double> { typedef double sum_type; };

    template<typename T>
    class average
    {
        private:
        size_t total;
        typename sum_traits<T>::sum_type sum;

        public:
        average ()
            : total (0)
            , sum (0)
        {
        }
        void update (const T &x)
        {
            ++total;
            sum += x;
        }
        double result ()
        {
            return static_cast<double> (sum) / total;
        }
    };

    template<typename T>
    class variance
    {
        private:
        size_t total;
        typename sum_traits<T>::sum_type sum;
        typename sum_traits<T>::sum_type sum2;

        public:
        variance ()
            : total (0)
            , sum (0)
            , sum2 (0)
        {
        }
        void update (const T &x)
        {
            ++total;
            sum += x;
            sum2 += (x * x);
        }
        double result ()
        {
            const double mean = static_cast<double> (sum) / total;
            return static_cast<double> (sum2) / total - mean * mean;
        }
    };

    template<typename T>
    class stddev
    {
        private:
        variance<T> v;

        public:
        void update (const T &x)
        {
            v.update (x);
        }
        double result ()
        {
            return sqrt (v.result ());
        }
    };

    template<typename T>
    using rms_contrast = stddev<T>;

    template<typename OP,typename T,typename U>
    void filter (const T &p, U &q, const size_t rows, const size_t cols, const size_t krows, const size_t kcols)
    {
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
                if (std::is_integral<typename U::value_type>::value)
                    q[n] = ::round (y);
                else
                    q[n] = y;
            }
        }
    }

    template<typename B,typename T,typename U>
    void filter (const T &p, U &q, const size_t rows, const size_t cols, const size_t k)
    {
        return filter<B,T,U> (p, q, rows, cols, k, k);
    }

    template<typename T>
    class fast_average
    {
        private:
        size_t total;
        typename sum_traits<T>::sum_type sum;

        public:
        fast_average ()
            : total (0)
            , sum (0)
        {
        }
        void update (const T &x)
        {
            ++total;
            sum += x;
        }
        double result ()
        {
            return static_cast<double> (sum) / total;
        }
    };

    template<typename T>
    class fast_variance
    {
        private:
        size_t total;
        typename sum_traits<T>::sum_type sum;
        typename sum_traits<T>::sum_type sum2;

        public:
        fast_variance ()
            : total (0)
            , sum (0)
            , sum2 (0)
        {
        }
        void update (const T &x)
        {
            ++total;
            sum += x;
            sum2 += (x * x);
        }
        double result ()
        {
            const double mean = static_cast<double> (sum) / total;
            return static_cast<double> (sum2) / total - mean * mean;
        }
    };

    template<typename T>
    class fast_stddev
    {
        private:
        fast_variance<T> v;

        public:
        void update (const T &x)
        {
            v.update (x);
        }
        double result ()
        {
            return sqrt (v.result ());
        }
    };

    template<typename T>
    using fast_rms_contrast = fast_stddev<T>;

    template<typename OP,typename T,typename U>
    void filter_row (const size_t row, const T &p, U &q, const size_t cols, const size_t kcols)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            OP op;
            const size_t n1 = index (row, j, cols);
            assert (n1 < p.size ());
            op.update (p[n1]);
            if (j + 1 < kcols)
                continue;
            // note that we are writing q transposed
            const size_t n2 = index (j - kcols / 2, row, cols);
            assert (n2 < q.size ());
            const double y = op.result ();
            if (std::is_integral<typename U::value_type>::value)
                q[n2] = ::round (y);
            else
                q[n2] = y;
        }
    }

    template<typename OP,typename T,typename U>
    void fast_filter (const T &p, U &q, const size_t rows, const size_t cols, const size_t krows, const size_t kcols)
    {
        U tmp (q);
        // pass 1: filter p's rows and write to tmp
        for (size_t i = 0; i < rows; ++i)
            filter_row<OP> (i, p, tmp, cols, kcols);

        // pass 2: filter tmp's rows and write to q
        for (size_t j = 0; j < cols; ++j)
            filter_row<OP> (j, tmp, q, rows, krows);
    }

    template<typename OP,typename T,typename U>
    void fast_filter (const T &p, U &q, const size_t rows, const size_t cols, const size_t k)
    {
        return fast_filter<OP,T,U> (p, q, rows, cols, k, k);
    }
}
