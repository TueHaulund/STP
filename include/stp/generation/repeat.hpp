#ifndef STP_REPEAT_HPP
#define STP_REPEAT_HPP

#include <vector>

namespace stp
{
    namespace detail
    {
        template <typename T>
        std::vector<T> repeat(const T &value, const size_t &n)
        {
            return std::vector<T>(n, value);
        }
    }

    template <typename T>
    std::vector<T> Repeat(const T &value, size_t n)
    {
        return detail::repeat(value, n);
    }
}

#endif