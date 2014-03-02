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
            typename TransformationType,
            typename ...Rest
        >
        class stp_type : public stp_type<Rest...>
        {
            public:
                using base_type = stp_type<Rest...>;

                template <typename SequenceType>
                struct return_traits
                {
                    using call_type = decltype(&TransformationType::template operator()<typename std::decay<SequenceType>::type>);
                    using transformation_traits = typename utility::functor_traits<call_type>;
                    using type = typename base_type::template return_traits<typename transformation_traits::return_type>::type;

                    static_assert(transformation_traits::pass_by_reference, "Transformation must accept parameters by reference");
                    static_assert(transformation_traits::return_by_reference, "Non-terminal transformations must return by reference");

                    //Sanity check
                    static_assert(std::is_same<TransformationType, typename transformation_traits::functor_type>::value, "Transformation type mismatch");
                };

                stp_type(TransformationType transform, Rest... rest) : base_type(rest...), transform_(transform)
                {}

                template <typename SequenceType>
                typename return_traits<SequenceType>::type operator()(SequenceType &input) const
                {
                    return base_type::operator()(transform_(input));
                }

            private:
                TransformationType transform_;
        };

        template <typename TransformationType>
        class stp_type<TransformationType>
        {
            public:
                template <typename SequenceType>
                struct return_traits
                {
                    using call_type = decltype(&TransformationType::template operator()<typename std::decay<SequenceType>::type>);
                    using transformation_traits = typename utility::functor_traits<call_type>;
                    using type = typename std::remove_reference<typename transformation_traits::return_type>::type;

                    static_assert(transformation_traits::pass_by_reference, "Transformation must accept parameters by reference");

                    //Sanity check
                    static_assert(std::is_same<TransformationType, typename transformation_traits::functor_type>::value, "Transformation type mismatch");
                };

                stp_type(TransformationType transform) : transform_(transform)
                {}

                template <typename SequenceType>
                typename return_traits<SequenceType>::type operator()(SequenceType &input) const
                {
                    return transform_(input);
                }

            private:
                TransformationType transform_;
        };

        template
        <
            typename TransformationType,
            typename ...Rest
        >
        class stp_wrapper : public stp_type<TransformationType, Rest...>
        {
            public:
                using base_type = stp_type<TransformationType, Rest...>;
                stp_wrapper(TransformationType transform, Rest... rest) : base_type(transform, rest...) {}

                template <typename SequenceType>
                typename base_type::template return_traits<SequenceType>::type operator()(SequenceType input) const
                {
                    return base_type::operator()(input);
                }
        };
    }

    template
    <
        typename TransformationType,
        typename ...Rest
    >
    detail::stp_wrapper<TransformationType, Rest...> make_stp(TransformationType &&transform, Rest&&... rest)
    {
        return detail::stp_wrapper<TransformationType, Rest...>(std::forward<TransformationType>(transform), std::forward<Rest>(rest)...);
    }

    //TODO: ARRAY & ITERATOR WRAPPER FUNCTIONS
}

#endif