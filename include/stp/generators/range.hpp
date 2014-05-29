#ifndef STP_RANGE_HPP
#define STP_RANGE_HPP

//abs overloads for int, long and long long
#include <cmath>
#include <cstdlib>
#include <type_traits>
#include <vector>

namespace stp
{
    namespace detail
    {
        template
        <
            typename IntervalType,
            typename StepType,
            typename = typename std::enable_if<std::is_scalar<IntervalType>::value>::type,
            typename = typename std::enable_if<std::is_scalar<StepType>::value>::type,
            typename RangeType = std::vector<IntervalType>
        >
        RangeType range_delegate(const IntervalType &start, const IntervalType &end, const StepType &step)
        {
            RangeType result;

            StepType u_zero = static_cast<StepType>(0);
            IntervalType range_diff = static_cast<IntervalType>(abs(end - start));
            IntervalType t_step = static_cast<IntervalType>(step);

            if(step == u_zero || range_diff < t_step)
            {
                return result;
            }

            if(start == end)
            {
                result.push_back(start);
            }
            else if(start < end)
            {
                for(IntervalType i = start; i < end; i += t_step)
                {
                    result.push_back(i);
                }
            }
            else
            {
                for(IntervalType i = start; i > end; i -= t_step)
                {
                    result.push_back(i);
                }
            }

            return result;
        }
    }

    template
    <
        typename IntervalType,
        typename StepType,
        typename RangeType = std::vector<IntervalType>
    >
    RangeType range(const IntervalType &start, const IntervalType &end, const StepType &step)
    {
        return detail::range_delegate<IntervalType, StepType>(start, end, step);
    }

    template
    <
        typename IntervalType,
        typename RangeType = std::vector<IntervalType>
    >
    RangeType range(const IntervalType &start, const IntervalType &end)
    {
        return detail::range_delegate<IntervalType, int>(start, end, 1);
    }
}

#endif