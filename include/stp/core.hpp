#ifndef STP_CORE_HPP
#define STP_CORE_HPP

#include <vector>
#include <list>
#include <iterator>
#include <algorithm>
#include <type_traits>

namespace stp
{
    template <typename FirstFunction, typename ...RestFunctions>
    class function_pipeline : public function_pipeline<RestFunctions...>
    {
        public:
            typedef function_pipeline<RestFunctions...> base_type;

            template <typename Input>
            struct return_type
            {
                typedef typename std::add_lvalue_reference<Input>::type ref_type;
                typedef typename std::result_of<FirstFunction(ref_type)>::type transformed_type;
                typedef typename base_type::template return_type<transformed_type>::type type;
            };

            function_pipeline(FirstFunction first, RestFunctions... rest) : base_type(rest...), func(first)
            {}

            template <typename Input>
            typename return_type<Input>::type operator()(Input &input)
            {
                return base_type::operator()(func(input));
            }

        private:
            FirstFunction func;
    };

    template <typename LastFunction>
    class function_pipeline<LastFunction>
    {
        public:
            template <typename Input>
            struct return_type
            {
                typedef typename std::add_lvalue_reference<Input>::type ref_type;
                typedef typename std::result_of<LastFunction(ref_type)>::type type;
            };

            function_pipeline(LastFunction last) : func(last)
            {}

            template <typename Input>
            typename return_type<Input>::type operator()(Input &input)
            {
                return func(input);
            }

        private:
            LastFunction func;
    };

    template <typename FirstFunction, typename ...RestFunctions>
    function_pipeline<FirstFunction, RestFunctions...> make_fp (FirstFunction &&first, RestFunctions&&... rest)
    {
        return function_pipeline<FirstFunction, RestFunctions...>(std::forward<FirstFunction>(first), std::forward<RestFunctions>(rest)...);
    }

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

    //TODO: ARRAY & ITERATOR OVERLOADS
}

#endif