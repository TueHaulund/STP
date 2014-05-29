#ifndef STP_MAP_HPP
#define STP_MAP_HPP

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <vector>

namespace stp
{
    namespace detail
    {
        template <typename UnaryOperation>
        struct map_type
        {
            map_type(const UnaryOperation &unop) : unop_(unop) {}

            template
            <
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type,
                typename OpType = typename std::result_of<UnaryOperation(ValueType)>::type,
                typename MapType = typename std::vector<OpType>
            >
            MapType operator()(const SequenceType &sequence) const
            {
                MapType result;

                auto begin = std::begin(sequence);
                auto end = std::end(sequence);

                result.resize(std::distance(begin, end));

                std::transform(begin, end, std::begin(result), unop_);

                return result;
            }

            UnaryOperation unop_;
        };
    }

    template <typename UnaryOperation>
    detail::map_type<UnaryOperation> map(const UnaryOperation &unop)
    {
        return detail::map_type<UnaryOperation>(unop);
    }
}

#endif