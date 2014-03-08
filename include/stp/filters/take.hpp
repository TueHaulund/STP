#ifndef STP_TAKE_HPP
#define STP_TAKE_HPP

#include <utility>
#include <iterator>
#include <algorithm>

namespace stp
{
    namespace detail
    {
        struct take_type
        {
            take_type(const size_t &n) : n_(n) {}

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

                input.erase(begin + std::min(n, std::distance(begin, end)), end);

                return input;
            }

            size_t n_;
        };

        template <typename Predicate>
        struct take_while_type
        {
            take_while_type(const Predicate &pred) : pred_(pred) {}

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

                take_type take_n(n);
                return take_n(input);
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