#ifndef STP_ALL_HPP
#define STP_ALL_HPP

#include <utility>
#include <algorithm>

namespace stp
{
    namespace detail
    {
        template <typename Predicate>
        struct all_type
        {
            all_type(const Predicate &pred) : pred_(pred) {}

            template <typename Input>
            bool operator()(Input &input) const
            {
                return std::all_of(std::begin(input), std::end(input), pred_);
            }

            Predicate pred_;
        };
    }

    template <typename Predicate>
    detail::all_type<Predicate> all(const Predicate &pred)
    {
        return detail::all_type<Predicate>(pred);
    }
}

#endif