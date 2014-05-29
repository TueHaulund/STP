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
        struct fold_type
        {
            fold_type(const BinaryOperation &binop, const InitType &init) : binop_(binop), init_(init) {}

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
    }

    template
    <
        typename BinaryOperation,
        typename InitType
    >
    detail::fold_type<BinaryOperation, InitType> fold(const BinaryOperation &binop, const InitType &init)
    {
        return detail::fold_type<BinaryOperation, InitType>(binop, init);
    }
}

#endif