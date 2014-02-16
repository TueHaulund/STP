#ifndef STP_SEQUENCE_SOURCES
#define STP_SEQUENCE_SOURCES

#include <cmath>
#include <vector>
#include <type_traits>

namespace stp
{
    namespace detail
    {
        /*template <typename T, size_t N>
        PartialTransformation<T> from_array(const T (&p_arr)[N])
        {
            const T *begin = p_arr;
            const T *end = begin + N;
            return from_iterators<T>(begin, end);
        }*/

        template <typename T,
                  typename U,
                  class = typename std::enable_if<std::is_arithmetic<T>::value>::type,
                  class = typename std::enable_if<!std::is_same<char, U>::value>::type>
        std::vector<T> range(const T &p_start, const T &p_end, const U &p_step)
        {
            std::vector<T> range_vec;

            if(p_step == static_cast<T>(0) || (static_cast<T>(abs(p_end - p_start)) < p_step))
                return range_vec;

            if(p_start == p_end)
            {
                range_vec.push_back(p_start);
            }
            else if(p_start < p_end)
            {
                for(T i = p_start; i < p_end; i += p_step)
                    range_vec.push_back(i);
            }
            else
            {
                for(T i = p_start; i > p_end; i -= p_step)
                    range_vec.push_back(i);
            }

            return range_vec;
        }

        template <typename T>
        std::vector<T> repeat(const T &value, size_t n)
        {
            return std::vector<T>(n, value);
        }
    }

    /*template <typename T, size_t N>
    PartialTransformation<T> FromArray(const T (&p_arr)[N])
    {
        return detail::from_array(p_arr);
    }*/

    template <typename T,
              typename U>
    std::vector<T> Range(const T &p_start, const T &p_end, const U &p_step)
    {
        return detail::range(p_start, p_end, p_step);
    }

    template <typename T>
    std::vector<T> Range(const T &p_start, const T &p_end)
    {
        return detail::range(p_start, p_end, 1);
    }

    template <typename T>
    std::vector<T> Repeat(const T &value, size_t n)
    {
        return detail::repeat(value, n);
    }
}

#endif