#ifndef STP_REVERSE_HPP
#define STP_REVERSE_HPP

#include <algorithm>
#include <iterator>

namespace stp
{
    namespace detail
    {
        struct reverse_type
        {
            template <typename SequenceType>
            SequenceType operator()(SequenceType sequence) const
            {
                std::reverse(std::begin(sequence), std::end(sequence));
                return sequence;
            }
        };
    }

    detail::reverse_type reverse()
    {
        return detail::reverse_type();
    }
}

#endif