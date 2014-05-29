#ifndef STP_DROP_HPP
#define STP_DROP_HPP

#include <algorithm>
#include <iterator>
#include <type_traits>

namespace stp
{
    namespace detail
    {
        struct drop_type
        {
            drop_type(const size_t &n) : n_(n) {}

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

                sequence.erase(begin, begin + std::min(n, std::distance(begin, end)));

                return sequence;
            }

            size_t n_;
        };

        template <typename Predicate>
        struct drop_while_type
        {
            drop_while_type(const Predicate &pred) : pred_(pred) {}

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

                drop_type drop_n(n);
                return drop_n(sequence);
            }

            Predicate pred_;
        };
    }

    detail::drop_type drop(const size_t &n)
    {
        return detail::drop_type(n);
    }

    template <typename Predicate>
    detail::drop_while_type<Predicate> drop_while(const Predicate &pred)
    {
        return detail::drop_while_type<Predicate>(pred);
    }
}

#endif