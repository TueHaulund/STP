#ifndef STP_REPEAT_HPP
#define STP_REPEAT_HPP

#include <vector>

namespace stp
{
    namespace detail
    {
        template
        <
            typename ValueType,
            typename RepeatType = typename std::vector<ValueType>
        >
        RepeatType repeat_delegate(const ValueType &val, const size_t &n)
        {
            return RepeatType(n, val);
        }
    }

    template
    <
        typename ValueType,
        typename RepeatType = typename std::vector<ValueType>
    >
    RepeatType repeat(const ValueType &val, size_t n)
    {
        return detail::repeat_delegate<ValueType>(val, n);
    }
}

#endif