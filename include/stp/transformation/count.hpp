#ifndef STP_COUNT_HPP
#define STP_COUNT_HPP

#include <utility>

namespace stp
{
    namespace detail
    {
        struct count_type
        {
            template <typename Input>
            size_t operator()(Input &input) const
            {
                auto begin = std::begin(input);
                auto end = std::end(input);

                return std::distance(begin, end);
            }
        };
    }

    detail::count_type count()
    {
        return detail::count_type();
    }
}

#endif