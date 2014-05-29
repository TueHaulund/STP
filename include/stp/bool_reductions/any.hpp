#ifndef STP_ANY_HPP
#define STP_ANY_HPP

#include <algorithm>
#include <iterator>
#include <type_traits>

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
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type,
                typename PredType = typename std::result_of<Predicate(ValueType)>::type,
                typename = typename std::enable_if<std::is_convertible<PredType, bool>::value>::type
            >
            bool operator()(const SequenceType &sequence) const
            {
                return std::any_of(std::begin(sequence), std::end(sequence), pred_);
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