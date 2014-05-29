#ifndef STP_INTERSECT_HPP
#define STP_INTERSECT_HPP

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
        struct intersect_type
        {
            intersect_type(const FixedSequenceType &sequence) : fixed_sequence_(sequence) {}

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
                for(;begin != end && begin_fixed != end_fixed; ++begin, ++begin_fixed)
                {
                    if(*begin == *begin_fixed)
                    {
                        result.push_back(*begin);
                    }
                }
                return result;
            }

            FixedSequenceType fixed_sequence_;
        };
    }

    template <typename SequenceType>
    detail::intersect_type<SequenceType> intersect(const SequenceType &sequence)
    {
        return detail::intersect_type<SequenceType>(sequence);
    }
}

#endif