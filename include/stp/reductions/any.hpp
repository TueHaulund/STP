#ifndef STP_ANY_HPP
#define STP_ANY_HPP

#include <utility>
#include <algorithm>

namespace stp
{
    namespace detail
    {
        template <typename Predicate>
        struct any_type
        {
            any_type(const Predicate &pred) : pred_(pred) {}

            template
            <
                typename Input,
                typename ValueType = typename Input::value_type
            >
            bool operator()(Input &input) const
            {
                return std::any_of(std::begin(input), std::end(input), pred_);
            }

            Predicate pred_;
        };
    }

    template <typename Predicate>
    detail::any_type<Predicate> any(const Predicate &pred)
    {
        return detail::any_type<Predicate>(pred);
    }
}

#endif