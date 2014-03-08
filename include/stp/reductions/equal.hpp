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
        template <typename Input1>
        struct equal_type
        {
            equal_type(const Input1 &sequence) : sequence_(sequence) {}

            template <typename Input2>
            bool operator()(const Input2 &input) const
            {
                auto begin1 = std::begin(sequence_);
                auto end1 = std::end(sequence_);

                auto begin2 = std::begin(input);
                auto end2 = std::end(input);

                if(std::distance(begin1, end1) != std::distance(begin2, end2))
                {
                    return false;
                }
                else
                {
                    return std::equal(begin1, end1, begin2);
                }
            }

            Input1 sequence_;
        };
    }

    template <typename Input>
    detail::equal_type<Input> equal(const Input &val)
    {
        return detail::equal_type<Input>(val);
    }
}

#endif