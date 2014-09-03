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

/*
    /// @brief 1D fast block sums in horizontal direction
    ///
    /// compute sums over 1xN blocks of an image
    ///
    /// @tparam T image type
    /// @param p image
    /// @param N block size
    ///
    /// @return image containing block sums
    template<typename T>
    jack_rabbit::raster<size_t> fast_block_sums (const T &p, const size_t N)
    {
        jack_rabbit::raster<size_t> q (p.rows (), p.cols ());
        for (size_t i = 0; i < p.rows (); ++i)
        {
            size_t sum = 0;
            size_t added = 0;
            for (size_t j = 0; j < p.cols (); ++j)
            {
                sum += p (i, j);
                ++added;
                if (added > N)
                {
                    sum -= p (i, j - N);
                    q (i, j - N / 2) = sum;
                }
            }
        }
        return q;
    }

    /// @brief 1D fast block sum of squares in horizontal direction
    ///
    /// compute sum of squares over 1xN blocks of an image
    ///
    /// @tparam T image type
    /// @param p image
    /// @param N block size
    ///
    /// @return image containing block sum of squares
    template<typename T>
    jack_rabbit::raster<size_t> fast_block_sum_squares (const T &p, const size_t N)
    {
        jack_rabbit::raster<size_t> q (p.rows (), p.cols ());
        for (size_t i = 0; i < p.rows (); ++i)
        {
            size_t sum = 0;
            size_t added = 0;
            for (size_t j = 0; j < p.cols (); ++j)
            {
                sum += p (i, j) * p (i, j);
                ++added;
                if (added > N)
                {
                    sum -= p (i, j - N) * p (i, j - N);
                    q (i, j - N / 2) = sum;
                }
            }
        }
        return q;
    }

    /// @brief Fast block average
    ///
    /// compute averages over NxN blocks of an image, clipping the edges
    ///
    /// @tparam T image type
    /// @param p image
    /// @param N block size
    ///
    /// @return image containing block averages
    template<typename T>
    T fast_block_average (const T &p, const size_t N)
    {
        // add a mirrored border
        const unsigned BORDER = N / 2 + 1;
        const T m = horny_toad::mborder<jack_rabbit::subregion> (p, BORDER);
        // get sums in 1d
        jack_rabbit::raster<size_t> q = horny_toad::transpose (fast_block_sums (m, N));
        // get sum of sums
        q = horny_toad::transpose (fast_block_sums (q, N));
        T r (q.rows (), q.cols ());
        for (size_t i = 0; i < q.size (); ++i)
            r[i] = round (q[i] / (N * N));
        return horny_toad::crop (r, BORDER);
    }

    /// @brief Fast block standard deviation
    ///
    /// compute standard deviations over NxN blocks of an image, clipping the edges
    ///
    /// @tparam T image type
    /// @param p image
    /// @param N block size
    ///
    /// @return image containing block standard deviations
    template<typename T>
    T fast_block_stddev (const T &p, const size_t N)
    {
        const unsigned BORDER = N / 2 + 1;
        const T m = horny_toad::mborder<jack_rabbit::subregion> (p, BORDER);
        const T a = fast_block_average (m, N);
        // get sum2s in 1d
        jack_rabbit::raster<size_t> q = horny_toad::transpose (fast_block_sum_squares (m, N));
        // get sum of sum2s
        q = horny_toad::transpose (fast_block_sums (q, N));
        T r (q.rows (), q.cols ());
        for (size_t i = 0; i < q.size (); ++i)
        {
            double s = q[i] / (N * N) - a[i] * a[i];
            if (s < 0.0)
                r[i] = 0;
            else
                r[i] = round (sqrt (s));
        }
        return horny_toad::crop (r, BORDER);
    }

    /// @brief Block average
    ///
    /// compute averages over NxN blocks of an image, clipping the edges
    ///
    /// @tparam T image type
    /// @param p image
    /// @param N block size
    /// @param c include center pixel
    ///
    /// @return image containing block averages
    template<typename T>
    T block_average (const T &p, const size_t N, bool c = true)
    {
        T q (p.rows (), p.cols ());
        for (size_t i = 0; i < p.rows (); ++i)
        {
            for (size_t j = 0; j < p.cols (); ++j)
            {
                size_t total = 0;
                double sum = 0;
                for (size_t i2 = 0; i2 < N; ++i2)
                {
                    size_t i3 = i - N / 2 + i2;
                    if (i3 >= p.rows ())
                        continue;
                    for (size_t j2 = 0; j2 < N; ++j2)
                    {
                        size_t j3 = j - N / 2 + j2;
                        if (j3 >= p.cols ())
                            continue;
                        // if specified, don't include the center
                        if (c == false && i3 == 0 && j3 == 0)
                            continue;
                        ++total;
                        assert (i3 < p.rows ());
                        assert (j3 < p.cols ());
                        sum += p (i3, j3);
                    }
                }
                q (i, j) = round (sum / total);
            }
        }
        return q;
    }

    /// @brief Block standard deviation
    ///
    /// compute standard deviations over NxN blocks of an image, clipping the edges
    ///
    /// @tparam T image type
    /// @param p image
    /// @param N block size
    /// @param c include center pixel
    ///
    /// @return image containing block standard deviations
    template<typename T>
    T block_stddev (const T &p, const size_t N, bool c = true)
    {
        T q (p.rows (), p.cols ());
        for (size_t i = 0; i < q.rows (); ++i)
        {
            for (size_t j = 0; j < q.cols (); ++j)
            {
                size_t total = 0;
                double sum = 0;
                double sum2 = 0;
                for (size_t i2 = 0; i2 < N; ++i2)
                {
                    size_t i3 = i - N / 2 + i2;
                    if (i3 >= p.rows ())
                        continue;
                    for (size_t j2 = 0; j2 < N; ++j2)
                    {
                        size_t j3 = j - N / 2 + j2;
                        if (j3 >= p.cols ())
                            continue;
                        // if specified, don't include the center
                        if (c == false && i3 == 0 && j3 == 0)
                            continue;
                        ++total;
                        assert (i3 < p.rows ());
                        assert (j3 < p.cols ());
                        sum += p (i3, j3);
                        sum2 += p (i3, j3) * p (i3, j3);
                    }
                }
                double avg = sum / total;
                double variance = sum2 / total - avg * avg;
                assert (variance >= 0.0);
                q (i, j) = round (sqrt (variance));
            }
        }
        return q;
    }
    */

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
        template<typename U>
        void update (const U &x)
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
        template<typename U>
        void update (const U &x)
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
        template<typename U>
        void update (const U &x)
        {
            v.update (x);
        }
        double result ()
        {
            return sqrt (v.result ());
        }
    };

    // Computing RMS contrast is equivalent to computing stddev
    template<typename T>
    using rms_contrast = stddev<T>;

    template<typename B,typename T,typename U>
    void filter (const T &p, U &q, const size_t rows, const size_t cols, const size_t krows, const size_t kcols)
    {
        for (size_t i = 0; i + krows < rows + 1; ++i)
        {
            for (size_t j = 0; j + kcols < cols + 1; ++j)
            {
                B op;
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
                    q[index (i + krows / 2, j + kcols / 2, cols)] = ::round (y);
                else
                    q[index (i + krows / 2, j + kcols / 2, cols)] = y;
            }
        }
    }

    template<typename B,typename T,typename U>
    void filter (const T &p, U &q, const size_t rows, const size_t cols, const size_t k)
    {
        return filter<B,T,U> (p, q, rows, cols, k, k);
    }

    template<typename B,typename T,typename U>
    void fast_filter (const T &p, U &q, const size_t rows, const size_t cols, const size_t krows, const size_t kcols)
    {
        for (size_t i = 0; i + krows < rows + 1; ++i)
        {
            for (size_t j = 0; j + kcols < cols + 1; ++j)
            {
                B op;
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
                    q[index (i + krows / 2, j + kcols / 2, cols)] = ::round (y);
                else
                    q[index (i + krows / 2, j + kcols / 2, cols)] = y;
            }
        }
    }

    template<typename B,typename T,typename U>
    void fast_filter (const T &p, U &q, const size_t rows, const size_t cols, const size_t k)
    {
        return fast_filter<B,T,U> (p, q, rows, cols, k, k);
    }
}
