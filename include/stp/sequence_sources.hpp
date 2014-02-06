#ifndef STP_SEQUENCE_SOURCES
#define STP_SEQUENCE_SOURCES

#include <cmath>

#include "core.hpp"

namespace stp
{
    namespace detail
    {
        template <typename T, typename InputIterator>
        PartialTransformation<T> from_iterators(InputIterator p_begin, InputIterator p_end)
        {
            return PartialTransformation<T>(p_begin, p_end);
        }

        template <template <typename T, typename... Ts> class Container, typename T, typename... Ts>
        PartialTransformation<T> from_container(const Container<T, Ts...> &p_container)
        {
            return from_iterators<T>(p_container.begin(), p_container.end());
        }

        template <typename T, size_t N>
        PartialTransformation<T> from_array(const T (&p_arr)[N])
        {
            const T *begin = p_arr;
            const T *end = begin + N;
            return from_iterators<T>(begin, end);
        }

        PartialTransformation<int> from_range(int p_start, int p_end, unsigned int p_step)
        {
            std::vector<int> range_vec;

            if(p_step == 0 || (static_cast<unsigned int>(abs(p_end - p_start)) < p_step))
                return from_container(range_vec);

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

            return from_container(range_vec);
        }
    }

    template <typename InputIterator>
    PartialTransformation<typename InputIterator::value_type> FromIterators(InputIterator p_begin, InputIterator p_end)
    {
        return detail::from_iterators<typename InputIterator::value_type>(p_begin, p_end);
    }

    template <template <typename T, typename... Ts> class Container, typename T, typename... Ts>
    PartialTransformation<T> FromContainer(const Container<T, Ts...> &p_container)
    {
        return detail::from_container(p_container);
    }

    template <typename T, size_t N>
    PartialTransformation<T> FromArray(const T (&p_arr)[N])
    {
        return detail::from_array(p_arr);
    }

    PartialTransformation<int> FromRange(int p_start, int p_end, int p_step = 1)
    {
        return detail::from_range(p_start, p_end, p_step);
    }
}

#endif