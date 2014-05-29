#ifndef STP_CONTAINS_HPP
#define STP_CONTAINS_HPP

#include <algorithm>
#include <iterator>
#include <type_traits>

namespace stp
{
    namespace detail
    {
        template <typename ElementType>
        struct contains_type
        {
            contains_type(const ElementType &val) : val_(val) {}

            template
            <
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type,
                typename = typename std::enable_if<std::is_convertible<ElementType, ValueType>::value>::type
            >
            bool operator()(const SequenceType &sequence) const
            {
                auto begin = std::begin(sequence);
                auto end = std::end(sequence);

                return std::find(begin, end, val_) != end;
            }

            ElementType val_;
        };
    }

    template <typename ElementType>
    detail::contains_type<ElementType> contains(const ElementType &val)
    {
        return detail::contains_type<ElementType>(val);
    }
}

#endif