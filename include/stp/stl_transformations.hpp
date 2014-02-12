#ifndef STP_STL_TRANSFORMATIONS_HPP
#define STP_STL_TRANSFORMATIONS_HPP

#include <vector>
#include <list>
#include <algorithm>

namespace stp
{
    struct ToVector
    {
        template <typename T>
        std::vector<typename T::value_type> operator()(T input)
        {
            std::vector<typename T::value_type> vec;
            std::for_each(input.begin(), input.end(), [&](const typename T::value_type &i){vec.push_back(std::move(i));});
            return vec;
        }
    };

    struct ToList
    {
        template <typename T>
        std::list<typename T::value_type> operator()(T input)
        {
            std::list<typename T::value_type> list;
            std::for_each(input.begin(), input.end(), [&](const typename T::value_type &i){list.push_back(std::move(i));});
            return list;
        }
    };
}

#endif