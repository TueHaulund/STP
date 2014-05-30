#ifndef STP_WHERE_HPP
#define STP_WHERE_HPP

#include <algorithm>
#include <iterator>
#include <type_traits>

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
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type,
                typename PredType = typename std::result_of<Predicate(ValueType)>::type,
                typename = typename std::enable_if<std::is_convertible<PredType, bool>::value>::type
            >
            SequenceType operator()(SequenceType sequence) const
            {
                auto neg_pred = [&](ValueType &i){return !pred_(i);};

                auto begin = std::begin(sequence);
                auto end = std::end(sequence);

                sequence.erase(std::remove_if(begin, end, neg_pred), end);
                return sequence;
            }

            Predicate pred_;
        };
    }

    template <typename Predicate>
    detail::where_type<Predicate> where(const Predicate &pred)
    {
        return detail::where_type<Predicate>(pred);
    }
}

#endif