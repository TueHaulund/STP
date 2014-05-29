#ifndef STP_SORT_HPP
#define STP_SORT_HPP

#include <algorithm>
#include <iterator>
#include <type_traits>

namespace stp
{
    namespace detail
    {
        struct sort_type
        {
            template <typename SequenceType>
            SequenceType& operator()(SequenceType &sequence) const
            {
                std::sort(std::begin(sequence), std::end(sequence));
                return sequence;
            }
        };

        template <typename Predicate>
        struct sort_with_type
        {
            sort_with_type(const Predicate &pred) : pred_(pred) {}

            template
            <
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type,
                typename PredType = typename std::result_of<Predicate(ValueType, ValueType)>::type,
                typename = typename std::enable_if<std::is_convertible<PredType, bool>::value>::type
            >
            SequenceType& operator()(SequenceType &sequence) const
            {
                std::sort(std::begin(sequence), std::end(sequence), pred_);
                return sequence;
            }

            Predicate pred_;
        };
    }

    detail::sort_type sort()
    {
        return detail::sort_type();
    }

    template <typename Predicate>
    detail::sort_with_type<Predicate> sort_with(const Predicate &pred)
    {
        return detail::sort_with_type<Predicate>(pred);
    }
}

#endif