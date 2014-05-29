#ifndef STP_MAX_HPP
#define STP_MAX_HPP

#include <algorithm>
#include <iterator>
#include <stdexcept>

namespace stp
{
    namespace detail
    {
        struct max_type
        {
            template
            <
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type
            >
            ValueType operator()(const SequenceType &sequence) const
            {
                auto begin = std::begin(sequence);
                auto end = std::end(sequence);

                if(begin == end)
                {
                    throw std::range_error("Empty sequence");
                }

                return *std::max_element(begin, end);
            }
        };
    }

    detail::max_type max()
    {
        return detail::max_type();
    }
}

#endif