#ifndef STP_FOLD_HPP
#define STP_FOLD_HPP

#include <iterator>
#include <numeric>
#include <utility>
#include <type_traits>

namespace stp
{
    namespace detail
    {
        template
        <
            typename BinaryOperation,
            typename InitType
        >
        struct foldl_type
        {
            foldl_type(const BinaryOperation &binop, const InitType &init) : binop_(binop), init_(init) {}

            template
            <
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type,
                typename = typename std::enable_if<std::is_convertible<InitType, ValueType>::value>::type
            >
            ValueType operator()(SequenceType &input) const
            {
                return std::accumulate(std::begin(input), std::end(input), init_, binop_);
            }

            BinaryOperation binop_;
            InitType init_;
        };

        template
        <
            typename BinaryOperation,
            typename InitType
        >
        struct foldr_type
        {
            foldr_type(const BinaryOperation &binop, const InitType &init) : binop_(binop), init_(init) {}

            template
            <
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type,
                typename ReverseIterType = typename std::reverse_iterator<typename SequenceType::const_iterator>,
                typename = typename std::enable_if<std::is_convertible<InitType, ValueType>::value>::type
            >
            ValueType operator()(const SequenceType &input) const
            {
                auto swap_args = [&](const ValueType &a, const ValueType &b){return binop_(b, a);};

                ReverseIterType rbegin(std::end(input)), rend(std::begin(input));
                return std::accumulate(rbegin, rend, init_, swap_args);
            }

            BinaryOperation binop_;
            InitType init_;
        };
    }

    template
    <
        typename BinaryOperation,
        typename InitType
    >
    detail::foldl_type<BinaryOperation, InitType> fold_left(const BinaryOperation &binop, const InitType &init)
    {
        return detail::foldl_type<BinaryOperation, InitType>(binop, init);
    }

    template
    <
        typename BinaryOperation,
        typename InitType
    >
    detail::foldr_type<BinaryOperation, InitType> fold_right(const BinaryOperation &binop, const InitType &init)
    {
        return detail::foldr_type<BinaryOperation, InitType>(binop, init);
    }
}

#endif