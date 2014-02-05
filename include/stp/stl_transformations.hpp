#ifndef STP_STL_TRANSFORMATIONS_HPP
#define STP_STL_TRANSFORMATIONS_HPP

#include <vector>
#include <list>

#include "core.hpp"

namespace stp
{
    struct ToVector
    {
        template <typename T>
        std::vector<T> operator()(PartialTransformation<T> p_pt)
        {
            return std::move(p_pt.data());
        }
    };

    struct ToList
    {
        template <typename T>
        std::list<T> operator()(PartialTransformation<T> p_pt)
        {
            return std::list<T>(p_pt.data().begin(), p_pt.data().end());
        }
    };
}

#endif