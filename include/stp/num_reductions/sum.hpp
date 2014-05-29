#ifndef STP_SUM_HPP
#define STP_SUM_HPP

#include <functional>
#include <numeric>
#include <type_traits>

namespace stp
{
    namespace detail
    {
        struct sum_type
        {
            template
            <
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type,
                typename = typename std::enable_if<std::is_default_constructible<ValueType>::value>::type
            >
            ValueType operator()(const SequenceType &sequence) const
            {
                return std::accumulate(std::begin(sequence), std::end(sequence), ValueType(), std::plus<ValueType>());
            }
        };
    }

    detail::sum_type sum()
    {
        return detail::sum_type();
    }
}

#endif