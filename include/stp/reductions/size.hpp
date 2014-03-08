#ifndef STP_SIZE_HPP
#define STP_SIZE_HPP

#include <utility>

namespace stp
{
    namespace detail
    {
        struct size_type
        {
            template
            <
                typename Input,
                typename IterType = typename Input::iterator,
                typename DiffType = typename std::iterator_traits<IterType>::difference_type
            >
            DiffType operator()(const Input &input) const
            {
                auto begin = std::begin(input);
                auto end = std::end(input);

                return std::distance(begin, end);
            }
        };
    }

    detail::size_type size()
    {
        return detail::size_type();
    }
}

#endif