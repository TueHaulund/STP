#ifndef STP_MIN_HPP
#define STP_MIN_HPP

#include <algorithm>
#include <iterator>
#include <stdexcept>

namespace stp
{
    namespace detail
    {
        struct min_type
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

                return *std::min_element(begin, end);
            }
        };
    }

    detail::min_type min()
    {
        return detail::min_type();
    }
}

#endif