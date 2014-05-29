#ifndef STP_AVG_HPP
#define STP_AVG_HPP

#include <iterator>
#include <stdexcept>

#include "sum.hpp"

namespace stp
{
    namespace detail
    {
        struct avg_type
        {
            template <typename SequenceType>
            double operator()(const SequenceType &sequence) const
            {
                auto begin = std::begin(sequence);
                auto end = std::end(sequence);

                if(begin == end)
                {
                    throw std::range_error("Empty sequence");
                }

                sum_type sum_obj;
                return static_cast<double>(sum_obj(sequence)) / static_cast<double>(std::distance(begin, end));
            }
        };
    }

    detail::avg_type avg()
    {
        return detail::avg_type();
    }
}

#endif