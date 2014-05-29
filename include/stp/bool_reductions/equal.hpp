#ifndef STP_EQUAL_HPP
#define STP_EQUAL_HPP

#include <algorithm>
#include <iterator>
#include <type_traits>

namespace stp
{
    namespace detail
    {
        template
        <
            typename FixedSequenceType,
            typename FixedValueType = typename FixedSequenceType::value_type
        >
        struct equal_type
        {
            equal_type(const FixedSequenceType &sequence) : fixed_sequence_(sequence) {}

            template
            <
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type,
                typename = typename std::enable_if<std::is_same<ValueType, FixedValueType>::value>::type
            >
            bool operator()(const SequenceType &sequence) const
            {
                auto begin = std::begin(sequence);
                auto end = std::end(sequence);

                auto begin_fixed = std::begin(fixed_sequence_);
                auto end_fixed = std::end(fixed_sequence_);

                if(std::distance(begin, end) == std::distance(begin_fixed, end_fixed))
                {
                    return std::equal(begin, end, begin_fixed);
                }

                return false;
            }

            FixedSequenceType fixed_sequence_;
        };
    }

    template <typename SequenceType>
    detail::equal_type<SequenceType> equal(const SequenceType &sequence)
    {
        return detail::equal_type<SequenceType>(sequence);
    }
}

#endif