#ifndef STP_FOLD_HPP
#define STP_FOLD_HPP

#include <iterator>
#include <numeric>
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
                typename OpType = typename std::result_of<BinaryOperation(InitType, ValueType)>::type,
                typename = typename std::enable_if<std::is_convertible<InitType, OpType>::value>::type
            >
            OpType operator()(const SequenceType &sequence) const
            {
                return std::accumulate(std::begin(sequence), std::end(sequence), init_, binop_);
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