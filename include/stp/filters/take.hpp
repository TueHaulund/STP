#ifndef STP_TAKE_HPP
#define STP_TAKE_HPP

#include <algorithm>
#include <iterator>
#include <type_traits>

namespace stp
{
    namespace detail
    {
        struct take_type
        {
            take_type(const size_t &n) : n_(n) {}

            template
            <
                typename SequenceType,
                typename IterType = typename SequenceType::iterator,
                typename DiffType = typename std::iterator_traits<IterType>::difference_type
            >
            SequenceType& operator()(SequenceType &sequence) const
            {
                auto begin = std::begin(sequence);
                auto end = std::end(sequence);

                DiffType n = static_cast<DiffType>(n_);

                sequence.erase(begin + std::min(n, std::distance(begin, end)), end);

                return sequence;
            }

            size_t n_;
        };

        template <typename Predicate>
        struct take_while_type
        {
            take_while_type(const Predicate &pred) : pred_(pred) {}

            template
            <
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type,
                typename PredType = typename std::result_of<Predicate(ValueType)>::type,
                typename = typename std::enable_if<std::is_convertible<PredType, bool>::value>::type
            >
            SequenceType& operator()(SequenceType &sequence) const
            {
                size_t n = 0;
                for(const ValueType &i : sequence)
                {
                    if(pred_(i))
                    {
                        n++;
                    }
                    else
                    {
                        break;
                    }
                }

                take_type take_n(n);
                return take_n(sequence);
            }

            Predicate pred_;
        };
    }

    detail::take_type take(const size_t &n)
    {
        return detail::take_type(n);
    }

    template <typename Predicate>
    detail::take_while_type<Predicate> take_while(const Predicate &pred)
    {
        return detail::take_while_type<Predicate>(pred);
    }
}

#endif