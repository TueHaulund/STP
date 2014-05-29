#ifndef STP_ALL_HPP
#define STP_ALL_HPP

#include <algorithm>
#include <iterator>
#include <type_traits>

namespace stp
{
    namespace detail
    {
        template <typename Predicate>
        struct all_type
        {
            all_type(const Predicate &pred) : pred_(pred) {}

            template
            <
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type,
                typename PredType = typename std::result_of<Predicate(ValueType)>::type,
                typename = typename std::enable_if<std::is_convertible<PredType, bool>::value>::type
            >
            bool operator()(const SequenceType &sequence) const
            {
                return std::all_of(std::begin(sequence), std::end(sequence), pred_);
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