#ifndef STP_WHERE_HPP
#define STP_WHERE_HPP

#include <utility>
#include <algorithm>

namespace stp
{
    namespace detail
    {
        template <typename Predicate>
        struct where_type
        {
            where_type(const Predicate &pred) : pred_(pred) {}

            template
            <
                typename Input,
                typename ValueType = typename Input::value_type
            >
            Input operator()(Input &input) const
            {
                auto neg_pred = [&](ValueType &i){return !pred_(i);};

                auto begin = std::begin(input);
                auto end = std::end(input);

                input.erase(std::remove_if(begin, end, neg_pred), end);
                return input;
            }

            Predicate pred_;
        };
    }

    template <typename Predicate>
    detail::where_type<Predicate> where(Predicate &&pred)
    {
        return detail::where_type<Predicate>(std::forward<Predicate>(pred));
    }
}

#endif