#ifndef STP_CORE_HPP
#define STP_CORE_HPP

#include <vector>
#include <list>
#include <iterator>
#include <algorithm>
#include <type_traits>

namespace stp
{
    /*template<typename Input, typename FirstTransformation, typename ...RestTransformations>
    struct Transformation : public Transformation<typename std::result_of<FirstTransformation(Input)>::type, RestTransformations...>
    {
        Transformation(Input p_input, FirstTransformation first_transformation, RestTransformations... rest) : base(first_transformation(p_input), rest...)
        {}

        private:
            typedef Transformation<typename std::result_of<FirstTransformation(Input)>::type, RestTransformations...> base;
    };

    template<typename Input, typename LastTransformation>
    struct Transformation<Input, LastTransformation>
    {
        Transformation(Input p_input, LastTransformation last_transformation) : m_result(last_transformation(p_input))
        {}

        typedef typename std::result_of<LastTransformation(Input)>::type transformed_type;
        transformed_type m_result;
    };

    template<typename Input, typename FirstTransformation, typename ...RestTransformations>
    typename Transformation<Input, FirstTransformation, RestTransformations...>::transformed_type Transform(Input input, FirstTransformation first_transformation, RestTransformations... rest)
    {
        Transformation<Input, FirstTransformation, RestTransformations...> transformation(input, first_transformation, rest...);
        return transformation.m_result;
    }*/

    /*template <typename> struct is_container : std::false_type {};
    template <template <typename...> class Container, typename... Ts> struct is_container<Container<Ts...>> : std::true_type {};*/

    /*template <typename T> struct element_type
    {
        typedef T type;
    };

    template <template <typename...> class Container, typename T, typename... Ts> struct element_type<Container<T, Ts...>>
    {
        typedef T type;
    };*/

    //template <typename> struct is_pq : std::false_type {};
    //template <typename T> struct is_pq<PartialTransformation<T>> : std::true_type {};

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