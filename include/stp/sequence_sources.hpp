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

        std::vector<int> range(int p_start, int p_end, unsigned int p_step)
        {
            std::vector<int> range_vec;

            if(p_step == 0 || (static_cast<unsigned int>(abs(p_end - p_start)) < p_step))
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
    }

    /*template <typename T, size_t N>
    PartialTransformation<T> FromArray(const T (&p_arr)[N])
    {
        return detail::from_array(p_arr);
    }*/

    std::vector<int> Range(int p_start, int p_end, int p_step = 1)
    {
        return detail::range(p_start, p_end, p_step);
    }
}

#endif