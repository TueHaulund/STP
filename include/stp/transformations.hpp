#ifndef STP_TRANSFORMATIONS_HPP
#define STP_TRANSFORMATIONS_HPP

#include <functional>
#include <algorithm>

namespace stp
{
    namespace detail
    {
        struct sum_type
        {
            template <typename Input>
            typename Input::value_type operator()(Input input)
            {
                typename Input::value_type sum = typename Input::value_type();
                std::for_each(input.begin(), input.end(), [&](const typename Input::value_type &i){sum += i;});
                return sum;
            }
        };

        struct take_type
        {
            take_type(const size_t &p_n) : n(p_n) {}

            template <typename Input>
            Input operator()(Input input)
            {
                if(input.size() >= n)
                    input.erase(input.begin() + n, input.end());

                return input;
            }

            size_t n;
        };

        template <typename Predicate>
        struct where_type
        {
            where_type(const Predicate &p_pred) : pred(p_pred) {}

            template <typename Input>
            Input operator()(Input input)
            {
                auto neg_pred = [&](const typename Input::value_type &i){return !pred(i);};

                input.erase(std::remove_if(input.begin(), input.end(), neg_pred), input.end());
                return input;
            }

            Predicate pred;
        };

        struct count_type
        {
            template <typename Input>
            size_t operator()(Input input)
            {
                return input.size();
            }
        };
    }

    detail::sum_type Sum()
    {
        return detail::sum_type();
    }

    detail::take_type Take(const size_t &p_n)
    {
        return detail::take_type(p_n);
    }

    template <typename Predicate>
    detail::where_type<Predicate> Where(const Predicate &p_pred)
    {
        return detail::where_type<Predicate>(p_pred);
    }

    detail::count_type Count()
    {
        return detail::count_type();
    }
}

#endif