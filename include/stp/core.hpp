#ifndef STP_CORE_HPP
#define STP_CORE_HPP

#include <vector>
#include <list>
#include <iterator>
#include <algorithm>
#include <type_traits>

namespace stp
{
    namespace detail
    {
        template
        <
            typename FirstFunction,
            typename ...RestFunctions
        >
        class stp_type : public stp_type<RestFunctions...>
        {
            public:
                typedef stp_type<RestFunctions...> base_type;

                template <typename Input>
                struct return_traits
                {
                    typedef typename std::add_lvalue_reference<Input>::type ref_type;
                    typedef typename std::result_of<FirstFunction(ref_type)>::type transformed_type;
                    typedef typename base_type::template return_traits<transformed_type>::type type;
                };

                stp_type(FirstFunction first, RestFunctions... rest) : base_type(rest...), func_(first)
                {}

                template <typename Input>
                typename return_traits<Input>::type operator()(Input &input)
                {
                    return base_type::operator()(func_(input));
                }

            private:
                FirstFunction func_;
        };

        template <typename LastFunction>
        class stp_type<LastFunction>
        {
            public:
                template <typename Input>
                struct return_traits
                {
                    typedef typename std::add_lvalue_reference<Input>::type ref_type;
                    typedef typename std::result_of<LastFunction(ref_type)>::type type;
                };

                stp_type(LastFunction last) : func_(last)
                {}

                template <typename Input>
                typename return_traits<Input>::type operator()(Input &input)
                {
                    return func_(input);
                }

            private:
                LastFunction func_;
        };
    }

    template
    <
        typename FirstFunction,
        typename ...RestFunctions
    >
    detail::stp_type<FirstFunction, RestFunctions...> make_stp (FirstFunction &&first, RestFunctions&&... rest)
    {
        return detail::stp_type<FirstFunction, RestFunctions...>(std::forward<FirstFunction>(first), std::forward<RestFunctions>(rest)...);
    }

    //TODO: ARRAY & ITERATOR WRAPPER FUNCTIONS

    //TEMP - REMOVE
    template<typename Input, typename Transformation, typename ...Transformations>
    struct return_type
    {
        typedef typename return_type<typename std::result_of<Transformation(Input)>::type, Transformations...>::type type;
    };

    template<typename Input, typename Transformation>
    struct return_type<Input, Transformation>
    {
        typedef typename std::result_of<Transformation(Input)>::type type;
    };

    template<typename Input>
    Input Transform(Input input)
    {
        return input;
    }

    template <typename Input, typename Transformation, typename... Transformations>
    typename return_type<Input, Transformation, Transformations...>::type Transform(Input input, Transformation first_transformation, Transformations... rest)
    {
        return Transform(std::move(first_transformation(std::move(input))), rest...);
    }
}

#endif