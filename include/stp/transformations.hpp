#ifndef STP_TRANSFORMATIONS_HPP
#define STP_TRANSFORMATIONS_HPP

#include <iterator>
#include <functional>
#include <algorithm>
#include <numeric>
#include <utility>

//TODO: CALL BY REFERENCE

namespace stp
{
    namespace detail
    {
        template <typename...>
        struct foldl_type;

        template
        <
            typename BinaryOperation,
            typename InitType
        >
        struct foldl_type<BinaryOperation, InitType>
        {
            foldl_type(const BinaryOperation &binop, const InitType &init) : binop_(binop), init_(init) {}

            template
            <
                typename Input,
                typename ValueType = typename Input::value_type,
                typename = typename std::enable_if<std::is_convertible<InitType, ValueType>::value>::type
            >
            ValueType operator()(Input input)
            {
                return std::accumulate(std::begin(input), std::end(input), init_, binop_);
            }

            BinaryOperation binop_;
            InitType init_;
        };

        template <typename BinaryOperation>
        struct foldl_type<BinaryOperation>
        {
            foldl_type(const BinaryOperation &binop) : binop_(binop) {}

            template
            <
                typename Input,
                typename ValueType = typename Input::value_type,
                typename = typename std::enable_if<std::is_default_constructible<ValueType>::value>::type
            >
            ValueType operator()(Input input)
            {
                return std::accumulate(std::begin(input), std::end(input), ValueType(), binop_);
            }

            BinaryOperation binop_;
        };

        struct sum_type
        {
            template
            <
                typename Input,
                typename ValueType = typename Input::value_type,
                typename = typename std::enable_if<std::is_default_constructible<ValueType>::value>::type
            >
            ValueType operator()(Input input)
            {
                return std::accumulate(std::begin(input), std::end(input), ValueType(), std::plus<ValueType>());
            }
        };

        struct take_type
        {
            take_type(const size_t &n) : n_(n) {}

            template
            <
                typename Input,
                typename InputIterator = typename Input::iterator,
                typename DiffType = typename std::iterator_traits<InputIterator>::difference_type
            >
            Input operator()(Input input)
            {
                auto begin = std::begin(input);
                auto end = std::end(input);

                DiffType n = static_cast<DiffType>(n_);

                if(std::distance(begin, end) >= n)
                    input.erase(begin + n, end);

                return input;
            }

            size_t n_;
        };

        template <typename Predicate>
        struct where_type
        {
            where_type(const Predicate &pred) : pred_(pred) {}

            template
            <
                typename Input,
                typename ValueType = typename Input::value_type
            >
            Input operator()(Input input)
            {
                auto neg_pred = [&](ValueType &i){return !pred_(i);};

                auto begin = std::begin(input);
                auto end = std::end(input);

                input.erase(std::remove_if(begin, end, neg_pred), end);
                return input;
            }

            Predicate pred_;
        };

        struct count_type
        {
            template <typename Input>
            size_t operator()(Input input)
            {
                auto begin = std::begin(input);
                auto end = std::end(input);

                return std::distance(begin, end);
            }
        };
    }

    template
    <
        typename BinaryOperation,
        typename InitType
    >
    detail::foldl_type<BinaryOperation, InitType> FoldLeft(BinaryOperation &&binop, InitType &&init)
    {
        return detail::foldl_type<BinaryOperation, InitType>(std::forward<BinaryOperation>(binop), std::forward<InitType>(init));
    }

    template
    <
        typename BinaryOperation
    >
    detail::foldl_type<BinaryOperation> FoldLeft(BinaryOperation &&binop)
    {
        return detail::foldl_type<BinaryOperation>(std::forward<BinaryOperation>(binop));
    }

    detail::sum_type Sum()
    {
        return detail::sum_type();
    }

    detail::take_type Take(size_t &&n)
    {
        return detail::take_type(std::forward<size_t>(n));
    }

    template <typename Predicate>
    detail::where_type<Predicate> Where(Predicate &&pred)
    {
        return detail::where_type<Predicate>(std::forward<Predicate>(pred));
    }

    detail::count_type Count()
    {
        return detail::count_type();
    }
}

#endif