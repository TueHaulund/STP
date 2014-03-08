#ifndef STP_DROP_HPP
#define STP_DROP_HPP

#include <utility>
#include <iterator>

namespace stp
{
    namespace detail
    {
        struct drop_type
        {
            drop_type(const size_t &n) : n_(n) {}

            template
            <
                typename Input,
                typename IterType = typename Input::iterator,
                typename DiffType = typename std::iterator_traits<IterType>::difference_type
            >
            Input& operator()(Input &input) const
            {
                auto begin = std::begin(input);
                auto end = std::end(input);

                DiffType n = static_cast<DiffType>(n_);

                if(std::distance(begin, end) >= n)
                {
                    input.erase(begin, begin + n);
                }
                else
                {
                    input.erase(begin, end);
                }

                return input;
            }

            size_t n_;
        };

        template <typename Predicate>
        struct drop_while_type
        {
            drop_while_type(const Predicate &pred) : pred_(pred) {}

            template
            <
                typename Input,
                typename ValueType = typename Input::value_type
            >
            Input& operator()(Input &input) const
            {
                size_t n = 0;
                for(const ValueType &i : input)
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
                return drop_n(input);
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