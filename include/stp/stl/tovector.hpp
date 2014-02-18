#ifndef STP_TOVECTOR_HPP
#define STP_TOVECTOR_HPP

#include <utility>
#include <vector>
#include <algorithm>

namespace stp
{
    namespace detail
    {
        struct to_vector_type
        {
            template
            <
                typename Input,
                typename ValueType = typename Input::value_type
            >
            std::vector<ValueType> operator()(Input input) const
            {
                std::vector<ValueType> vec;
                std::for_each(std::begin(input), std::end(input), [&](const ValueType &i){vec.push_back(std::move(i));});
                return vec;
            }
        };
    }

    detail::to_vector_type to_vector()
    {
        return detail::to_vector_type();
    }
}

#endif