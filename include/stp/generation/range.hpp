#ifndef STP_RANGE_HPP
#define STP_RANGE_HPP

#include <vector>
#include <type_traits>

namespace stp
{
    namespace detail
    {
        template
        <
            typename T,
            typename U,
            typename = typename std::enable_if<std::is_scalar<T>::value>::type,
            typename = typename std::enable_if<std::is_scalar<U>::value>::type
        >
        std::vector<T> range(const T &start, const T &end, const U &step)
        {
            std::vector<T> range_vec;

            U u_zero = static_cast<U>(0);
            T range_diff = static_cast<T>(abs(end - start));
            T t_step = static_cast<T>(step);

            if(step == u_zero || range_diff < t_step)
            {
                return range_vec;
            }

            if(start == end)
            {
                range_vec.push_back(start);
            }
            else if(start < end)
            {
                for(T i = start; i < end; i += t_step)
                {
                    range_vec.push_back(i);
                }
            }
            else
            {
                for(T i = start; i > end; i -= t_step)
                {
                    range_vec.push_back(i);
                }
            }

            return range_vec;
        }
    }

    template <typename T,
              typename U>
    std::vector<T> Range(const T &start, const T &end, const U &step)
    {
        return detail::range(start, end, step);
    }

    template <typename T>
    std::vector<T> Range(const T &start, const T &end)
    {
        return detail::range(start, end, 1);
    }
}

#endif