#ifndef STP_SIZE_HPP
#define STP_SIZE_HPP

#include <iterator>
#include <type_traits>

namespace stp
{
    namespace detail
    {
        struct size_type
        {
            template
            <
                typename SequenceType,
                typename IterType = typename SequenceType::iterator,
                typename DiffType = typename std::iterator_traits<IterType>::difference_type
            >
            DiffType operator()(const SequenceType &sequence) const
            {
                auto begin = std::begin(sequence);
                auto end = std::end(sequence);

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