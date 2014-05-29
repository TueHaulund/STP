#ifndef STP_ZIP_HPP
#define STP_ZIP_HPP

#include <iterator>
#include <utility>
#include <vector>

namespace stp
{
    namespace detail
    {
        template
        <
            typename FixedSequenceType,
            typename FixedValueType = typename FixedSequenceType::value_type
        >
        struct zip_type
        {
            zip_type(const FixedSequenceType &sequence) : fixed_sequence_(sequence) {}

            template
            <
                typename SequenceType,
                typename ValueType = typename SequenceType::value_type,
                typename PairType = typename std::pair<ValueType, FixedValueType>,
                typename ZipType = typename std::vector<PairType>
            >
            ZipType operator()(const SequenceType &sequence) const
            {
                auto begin = std::begin(sequence);
                auto end = std::end(sequence);

                auto begin_fixed = std::begin(fixed_sequence_);
                auto end_fixed = std::end(fixed_sequence_);

                ZipType result;
                if(std::distance(begin, end) < std::distance(begin_fixed, end_fixed))
                {
                    for(;begin != end; ++begin, ++begin_fixed)
                    {
                        result.push_back(std::make_pair(*begin, *begin_fixed));
                    }
                }
                else
                {
                    for(;begin_fixed != end_fixed; ++begin, ++begin_fixed)
                    {
                        result.push_back(std::make_pair(*begin, *begin_fixed));
                    }
                }
                return result;
            }

            FixedSequenceType fixed_sequence_;
        };
    }

    template <typename SequenceType>
    detail::zip_type<SequenceType> zip(const SequenceType &sequence)
    {
        return detail::zip_type<SequenceType>(sequence);
    }
}

#endif