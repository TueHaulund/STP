#ifndef STP_UNIQUE_HPP
#define STP_UNIQUE_HPP

#include <algorithm>
#include <iterator>

namespace stp
{
    namespace detail
    {
        struct unique_type
        {
            template
            <
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type
            >
            SequenceType operator()(const SequenceType &sequence) const
            {
                SequenceType result;
                for(const ValueType& i : sequence)
                {
                    if(std::find(std::begin(result), std::end(result), i) == std::end(result))
                    {
                        result.push_back(i);
                    }
                }

                return result;
            }
        };
    }

    detail::unique_type unique()
    {
        return detail::unique_type();
    }
}

#endif