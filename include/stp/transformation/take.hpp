#ifndef STP_TAKE_HPP
#define STP_TAKE_HPP

#include <utility>
#include <iterator>

namespace stp
{
    namespace detail
    {
        struct take_type
        {
            take_type(const size_t &n) : n_(n) {}

            template
            <
                typename Input,
                typename InputIterator = typename Input::iterator,
                typename DiffType = typename std::iterator_traits<InputIterator>::difference_type
            >
            Input operator()(Input input) const
            {
                auto begin = std::begin(input);
                auto end = std::end(input);

                DiffType n = static_cast<DiffType>(n_);

                if(std::distance(begin, end) >= n)
                    input.erase(begin + n, end);

                return input;
            }

            size_t n_;
        };
    }

    detail::take_type take(size_t &&n)
    {
        return detail::take_type(std::forward<size_t>(n));
    }
}

#endif