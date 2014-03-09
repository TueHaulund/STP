#ifndef STP_COUNT_HPP
#define STP_COUNT_HPP

#include <utility>
#include <iterator>
#include <algorithm>
#include <type_traits>

namespace stp
{
    namespace detail
    {
        template <typename ElementType>
        struct count_type
        {
            count_type(const ElementType &val) : val_(val) {}

            template
            <
                typename SequenceType,
                typename IterType = typename SequenceType::iterator,
                typename DiffType = typename std::iterator_traits<IterType>::difference_type,
                typename ValueType = typename SequenceType::value_type,
                typename = typename std::enable_if<std::is_convertible<ElementType, ValueType>::value>::type
            >
            DiffType operator()(const SequenceType &input) const
            {
                auto begin = std::begin(input);
                auto end = std::end(input);

                return std::count(begin, end, val_);
            }

            ElementType val_;
        };
    }

    template <typename ElementType>
    detail::count_type<ElementType> count(const ElementType &val)
    {
        return detail::count_type<ElementType>(val);
    }
}

#endif