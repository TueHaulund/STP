#ifndef STP_TRANSFORMATIONS_HPP
#define STP_TRANSFORMATIONS_HPP

#include <functional>
#include <algorithm>

#include "core.hpp"

namespace stp
{
    namespace detail
    {
        struct sum_type
        {
            template <typename T>
            T operator()(PartialTransformation<T> p_pt)
            {
                T sum = T();
                std::for_each(p_pt.data().begin(), p_pt.data().end(), [&](const T &i){sum += i;});
                return sum;
            }
        };

        struct take_type
        {
            take_type(const size_t &p_n) : n(p_n) {}

            template <typename T>
            PartialTransformation<T> operator()(PartialTransformation<T> p_pt)
            {
                if(p_pt.data().size() >= n)
                    p_pt.data().erase(p_pt.data().begin() + n, p_pt.data().end());

                return p_pt;
            }

            size_t n;
        };

        template <typename Predicate>
        struct where_type
        {
            where_type(const Predicate &p_pred) : pred(p_pred) {}

            template <typename T>
            PartialTransformation<T> operator()(PartialTransformation<T> p_pt)
            {
                auto neg_pred = [&](const T &i){return !pred(i);};
                auto begin = p_pt.data().begin();
                auto end = p_pt.data().end();

                p_pt.data().erase(std::remove_if(begin, end, neg_pred), end);
                return p_pt;
            }

            Predicate pred;
        };

        struct count_type
        {
            template <typename T>
            size_t operator()(PartialTransformation<T> p_pt)
            {
                return p_pt.data().size();
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