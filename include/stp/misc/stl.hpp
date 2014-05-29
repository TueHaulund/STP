#ifndef STP_STL_HPP
#define STP_STL_HPP

#include <algorithm>
#include <iterator>
#include <list>
#include <vector>

namespace stp
{
    namespace detail
    {
        struct to_vector_type
        {
            template
            <
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type,
                typename VectorType = typename std::vector<ValueType>
            >
            VectorType operator()(const SequenceType &input) const
            {
                VectorType result;
                std::for_each(std::begin(input), std::end(input), [&](const ValueType &i){result.push_back(i);});
                return result;
            }
        };

        struct to_list_type
        {
            template
            <
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type,
                typename ListType = typename std::list<ValueType>
            >
            ListType operator()(const SequenceType &input) const
            {
                ListType result;
                std::for_each(std::begin(input), std::end(input), [&](const ValueType &i){result.push_back(i);});
                return result;
            }
        };
    }

    detail::to_vector_type to_vector()
    {
        return detail::to_vector_type();
    }

    detail::to_list_type to_list()
    {
        return detail::to_list_type();
    }
}

#endif