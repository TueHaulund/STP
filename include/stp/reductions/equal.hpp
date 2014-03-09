#ifndef STP_EQUAL_HPP
#define STP_EQUAL_HPP

#include <utility>
#include <iterator>
#include <algorithm>
#include <type_traits>

namespace stp
{
    namespace detail
    {
        template <typename SequenceType1>
        struct equal_type
        {
            equal_type(const SequenceType1 &sequence) : sequence_(sequence) {}

            template <typename SequenceType2>
            bool operator()(const SequenceType2 &input) const
            {
                auto begin1 = std::begin(sequence_);
                auto end1 = std::end(sequence_);

                auto begin2 = std::begin(input);
                auto end2 = std::end(input);

                if(std::distance(begin1, end1) == std::distance(begin2, end2))
                {
                    return std::equal(begin1, end1, begin2);
                }

                return false;
            }

            SequenceType1 sequence_;
        };
    }

    template <typename Input>
    detail::equal_type<Input> equal(const Input &val)
    {
        return detail::equal_type<Input>(val);
    }
}

#endif