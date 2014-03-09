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

        template <typename BinaryOperation>
        struct foldl_type<BinaryOperation>
        {
            foldl_type(const BinaryOperation &binop) : binop_(binop) {}

            template
            <
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type,
                typename = typename std::enable_if<std::is_default_constructible<ValueType>::value>::type
            >
            ValueType operator()(SequenceType &input) const
            {
                return std::accumulate(std::begin(input), std::end(input), ValueType(), binop_);
            }

            BinaryOperation binop_;
        };

        template <typename...>
        struct foldr_type;

        template
        <
            typename BinaryOperation,
            typename InitType
        >
        struct foldr_type<BinaryOperation, InitType>
        {
            foldr_type(const BinaryOperation &binop, const InitType &init) : binop_(binop), init_(init) {}

            template
            <
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type,
                typename IterType = typename SequenceType::iterator,
                typename ReverseIterType = typename std::reverse_iterator<IterType>,
                typename = typename std::enable_if<std::is_convertible<InitType, ValueType>::value>::type
            >
            ValueType operator()(const SequenceType &input) const
            {
                auto rbegin = ReverseIterType(std::begin(input));
                auto rend = ReverseIterType(std::end(input));

                return std::accumulate(rbegin, rend, init_, binop_);
            }

            BinaryOperation binop_;
            InitType init_;
        };

        template <typename BinaryOperation>
        struct foldr_type<BinaryOperation>
        {
            foldr_type(const BinaryOperation &binop) : binop_(binop) {}

            template
            <
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type,
                typename IterType = typename SequenceType::iterator,
                typename ReverseIterType = typename std::reverse_iterator<IterType>,
                typename = typename std::enable_if<std::is_default_constructible<ValueType>::value>::type
            >
            ValueType operator()(const SequenceType &input) const
            {
                auto rbegin = ReverseIterType(std::begin(input));
                auto rend = ReverseIterType(std::end(input));

                return std::accumulate(rbegin, rend, ValueType(), binop_);
            }

            BinaryOperation binop_;
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
        typename BinaryOperation
    >
    detail::foldl_type<BinaryOperation> fold_left(const BinaryOperation &binop)
    {
        return detail::foldl_type<BinaryOperation>(binop);
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

    template
    <
        typename BinaryOperation
    >
    detail::foldr_type<BinaryOperation> fold_right(const BinaryOperation &binop)
    {
        return detail::foldr_type<BinaryOperation>(binop);
    }
}

#endif