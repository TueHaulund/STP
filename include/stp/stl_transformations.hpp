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

    template <typename T>
    PartialTransformation<T> FromVector(const std::vector<T> &p_vec)
    {
        return PartialTransformation<T>(p_vec.begin(), p_vec.end());
    }

    template <typename T>
    PartialTransformation<T> FromList(const std::list<T> &p_list)
    {
        return PartialTransformation<T>(p_list.begin(), p_list.end());
    }
}

#endif