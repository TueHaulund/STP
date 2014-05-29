#ifndef STP_DIFFERENCE_HPP
#define STP_DIFFERENCE_HPP

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
        struct difference_type
        {
            difference_type(const FixedSequenceType &sequence) : fixed_sequence_(sequence) {}

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
                for(;begin != end; ++begin)
                {
                    if(begin_fixed == end_fixed)
                    {
                        result.push_back(*begin);
                        continue;
                    }
                    else if(*begin != *begin_fixed)
                    {
                        result.push_back(*begin);
                    }
                    ++begin_fixed;
                }
                return result;
            }

            FixedSequenceType fixed_sequence_;
        };
    }

    template <typename SequenceType>
    detail::difference_type<SequenceType> difference(const SequenceType &sequence)
    {
        return detail::difference_type<SequenceType>(sequence);
    }
}

#endif