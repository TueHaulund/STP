#ifndef STP_CORE_HPP
#define STP_CORE_HPP

#include <vector>
#include <list>
#include <iterator>
#include <algorithm>
#include <type_traits>

#include "utility.hpp"

namespace stp
{
    namespace detail
    {
        template
        <
            typename Transformation,
            typename ...Rest
        >
        class stp_type : public stp_type<Rest...>
        {
            public:
                using base_type = stp_type<Rest...>;

                template <typename SequenceType>
                struct return_traits
                {
                    using transformation_traits = typename utility::functor_traits<Transformation, SequenceType>;
                    using type = typename base_type::template return_traits<typename transformation_traits::return_type>::type;

                    static_assert(transformation_traits::pass_by_reference, "Transformation must accept parameters by reference");
                    static_assert(transformation_traits::return_by_reference, "Non-terminal transformations must return by reference");

                    //Sanity check
                    static_assert(std::is_same<Transformation, typename transformation_traits::functor_type>::value, "Transformation type mismatch");
                };

                stp_type(Transformation transform, Rest... rest) : base_type(rest...), transform_(transform)
                {}

                template <typename SequenceType>
                typename return_traits<SequenceType>::type operator()(SequenceType &input) const
                {
                    return base_type::operator()(transform_(input));
                }

            private:
                Transformation transform_;
        };

        template <typename Transformation>
        class stp_type<Transformation>
        {
            public:
                template <typename SequenceType>
                struct return_traits
                {
                    using transformation_traits = typename utility::functor_traits<Transformation, SequenceType>;
                    using type = typename std::remove_reference<typename transformation_traits::return_type>::type;

                    static_assert(transformation_traits::pass_by_reference, "Transformation must accept parameters by reference");

                    //Sanity check
                    static_assert(std::is_same<Transformation, typename transformation_traits::functor_type>::value, "Transformation type mismatch");
                };

                stp_type(Transformation transform) : transform_(transform)
                {}

                template <typename SequenceType>
                typename return_traits<SequenceType>::type operator()(SequenceType &input) const
                {
                    return transform_(input);
                }

            private:
                Transformation transform_;
        };

        template
        <
            typename Transformation,
            typename ...Rest
        >
        class stp_wrapper : public stp_type<Transformation, Rest...>
        {
            public:
                using base_type = stp_type<Transformation, Rest...>;
                stp_wrapper(Transformation transform, Rest... rest) : base_type(transform, rest...) {}

                template <typename SequenceType>
                typename base_type::template return_traits<SequenceType>::type operator()(SequenceType input) const
                {
                    return base_type::operator()(input);
                }
        };
    }

    template
    <
        typename Transformation,
        typename ...Rest
    >
    detail::stp_wrapper<Transformation, Rest...> make_stp(Transformation &&transform, Rest&&... rest)
    {
        return detail::stp_wrapper<Transformation, Rest...>(std::forward<Transformation>(transform), std::forward<Rest>(rest)...);
    }

    //TODO: ARRAY & ITERATOR WRAPPER FUNCTIONS
}

#endif