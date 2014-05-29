#ifndef STP_JOIN_HPP
#define STP_JOIN_HPP

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
        struct join_type
        {
            join_type(const FixedSequenceType &sequence) : fixed_sequence_(sequence) {}

            template
            <
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type,
                typename = typename std::enable_if<std::is_same<ValueType, FixedValueType>::value>::type
            >
            SequenceType operator()(const SequenceType &sequence) const
            {
                auto begin = std::begin(sequence);
                auto end = std::end(sequence);

                auto begin_fixed = std::begin(fixed_sequence_);
                auto end_fixed = std::end(fixed_sequence_);

                SequenceType result;
                result.reserve(std::distance(begin, end) + std::distance(begin_fixed, end_fixed));
                result.insert(std::end(result), begin, end);
                result.insert(std::end(result), begin_fixed, end_fixed);
                return result;
            }

            FixedSequenceType fixed_sequence_;
        };
    }

    template <typename SequenceType>
    detail::join_type<SequenceType> join(const SequenceType &sequence)
    {
        return detail::join_type<SequenceType>(sequence);
    }
}

#endif