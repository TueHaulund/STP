#ifndef STP_STL_TRANSFORMATIONS_HPP
#define STP_STL_TRANSFORMATIONS_HPP

#include <vector>
#include <list>
#include <iterator>

#include "core.hpp"

namespace stp
{
    struct ToVector
    {
        template <typename T>
        std::vector<T> operator()(partial_query<T> pq)
        {
            return std::move(pq.data());
        }
    };

    struct ToList
    {
        template <typename T>
        std::list<T> operator()(partial_query<T> pq)
        {
            return std::list<T> {std::begin(pq.data()), std::end(pq.data())};
        }
    };

    template <typename T>
    partial_query<T> FromVector(const std::vector<T> &p_vec)
    {
        return partial_query<T>(p_vec.begin(), p_vec.end());
    }

    template <typename T>
    partial_query<T> FromList(const std::list<T> &p_list)
    {
        return partial_query<T>(p_list.begin(), p_list.end());
    }
}

#endif