#ifndef STP_SUM_HPP
#define STP_SUM_HPP

#include <functional>
#include <numeric>
#include <utility>
#include <type_traits>

namespace stp
{
    namespace detail
    {
        struct sum_type
        {
            template
            <
                typename Input,
                typename ValueType = typename Input::value_type,
                typename = typename std::enable_if<std::is_default_constructible<ValueType>::value>::type
            >
            ValueType operator()(Input &input) const
            {
                return std::accumulate(std::begin(input), std::end(input), ValueType(), std::plus<ValueType>());
            }
        };
    }

    detail::sum_type sum()
    {
        return detail::sum_type();
    }
}

#endif