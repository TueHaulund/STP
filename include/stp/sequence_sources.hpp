#ifndef STP_SEQUENCE_SOURCES
#define STP_SEQUENCE_SOURCES

#include <cmath>
#include <vector>

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

        template <typename T>
        std::vector<T> range(const T &p_start, const T &p_end, const T &p_step)
        {
            std::vector<T> range_vec;

            if(p_step == 0 || (abs(p_end - p_start) < p_step))
                return range_vec;

            if(p_start == p_end)
            {
                range_vec.push_back(p_start);
            }
            else if(p_start < p_end)
            {
                for(int i = p_start; i < p_end; i += p_step)
                    range_vec.push_back(i);
            }
            else
            {
                for(int i = p_start; i > p_end; i -= p_step)
                    range_vec.push_back(i);
            }

            return range_vec;
        }

        std::vector<char> range(char p_start, char p_end, int p_step)
        {
            std::vector<char> range_vec;

            if (p_step == 0 || static_cast<unsigned int>(abs((int)p_end - (int)p_start) < p_step))
                return range_vec;

            if ((int)p_start == (int)p_end)
            {
                range_vec.push_back(p_start);
            }
            else if ((int)p_start < (int)p_end)
            {
                for (int i = (int)p_start; i < (int)p_end; i += p_step)
                    range_vec.push_back((char)i);
            }
            else if ((int)p_end < (int)p_start)
            {
               for (int i = (int)p_start; i > (int)p_end; i -= p_step)
                    range_vec.push_back((char)i); 
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

    template <typename T>
    std::vector<T> Range(const T &p_start, const T &p_end, const T &p_step = 1)
    {
        return detail::range(p_start, p_end, p_step);
    }
    
    std::vector<char> Range(const char &p_start, const char &p_end, const int &p_step = 1)
    {
        return detail::range(p_start, p_end, p_step);
    }

    template <typename T>
    std::vector<T> Repeat(const T &value, size_t n)
    {
        return detail::repeat(value, n);
    }
}

#endif