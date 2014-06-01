#ifndef STP_STL_HPP
#define STP_STL_HPP

#include <algorithm>
#include <iterator>
#include <list>
#include <map>
#include <utility>
#include <vector>

namespace stp
{
    namespace detail
    {
        struct to_list_type
        {
            template
            <
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type,
                typename ListType = typename std::list<ValueType>
            >
            ListType operator()(const SequenceType &sequence) const
            {
                ListType result;
                std::for_each(std::begin(sequence), std::end(sequence), [&](const ValueType &i){result.push_back(i);});
                return result;
            }
        };

        struct to_map_type
        {
            template
            <
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type,
                typename FirstType = typename ValueType::first_type,
                typename SecondType = typename ValueType::second_type,
                typename PairType = typename std::pair<FirstType, SecondType>,
                typename MapType = typename std::map<FirstType, SecondType>
            >
            MapType operator()(const SequenceType &sequence) const
            {
                MapType result;
                std::for_each(std::begin(sequence), std::end(sequence), [&](const ValueType &i){result.insert(PairType(i.first, i.second));});
                return result;
            }
        };

        struct to_vector_type
        {
            template
            <
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type,
                typename VectorType = typename std::vector<ValueType>
            >
            VectorType operator()(const SequenceType &sequence) const
            {
                VectorType result;
                std::for_each(std::begin(sequence), std::end(sequence), [&](const ValueType &i){result.push_back(i);});
                return result;
            }
        };
    }

    detail::to_list_type to_list()
    {
        return detail::to_list_type();
    }

    detail::to_map_type to_map()
    {
        return detail::to_map_type();
    }

    detail::to_vector_type to_vector()
    {
        return detail::to_vector_type();
    }
}

#endif