#ifndef STP_STL_TRANSFORMATIONS_HPP
#define STP_STL_TRANSFORMATIONS_HPP

#include <vector>
#include <list>
#include <algorithm>

namespace stp
{
    struct ToVector
    {
        template <typename Input>
        std::vector<typename Input::value_type> operator()(Input input)
        {
            std::vector<typename Input::value_type> vec;
            std::for_each(input.begin(), input.end(), [&](const typename Input::value_type &i){vec.push_back(std::move(i));});
            return vec;
        }
    };

    struct ToList
    {
        template <typename Input>
        std::list<typename Input::value_type> operator()(Input input)
        {
            std::list<typename Input::value_type> list;
            std::for_each(input.begin(), input.end(), [&](const typename Input::value_type &i){list.push_back(std::move(i));});
            return list;
        }
    };
}

#endif