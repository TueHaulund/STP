#ifndef STP_CORE_HPP
#define STP_CORE_HPP

#include <vector>
#include <list>
#include <iterator>
#include <algorithm>
#include <type_traits>

namespace stp
{
    template <typename T>
    class PartialTransformation
    {
        public:
            typedef T value_type;

            template<typename InputIterator>
            PartialTransformation(InputIterator p_begin, InputIterator p_end)
            {
                std::copy(p_begin, p_end, std::back_inserter(transformation_data));
            }

            PartialTransformation(const PartialTransformation<T> &p_other)
            {
                transformation_data = p_other.transformation_data;
                std::cout << "Copy" << std::endl;
            }

            PartialTransformation(PartialTransformation<T>&& p_other)
            {
                transformation_data = std::move(p_other.transformation_data);
            }

            std::vector<T>& data()
            {
                return transformation_data;
            }

        private:
            std::vector<T> transformation_data;
    };

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

    template<typename T, typename Transformation, typename ...Transformations>
    struct return_type;

    template<typename T, typename Transformation>
    struct return_type<T, Transformation>
    {
        typedef typename std::result_of<Transformation(PartialTransformation<T>)>::type type;
    };

    template<typename T, typename Transformation, typename ...Transformations>
    struct return_type
    {
        typedef typename std::result_of<Transformation(PartialTransformation<T>)>::type transformed_type;
        typedef typename return_type<typename transformed_type::value_type, Transformations...>::type type;
    };

    template<typename T, typename Transformation>
    typename return_type<T, Transformation>::type Transform(PartialTransformation<T> pt, Transformation last_transformation)
    {
        return last_transformation(std::move(pt));
    }

    template <typename T, typename Transformation, typename... Transformations>
    typename return_type<T, Transformation, Transformations...>::type Transform(PartialTransformation<T> pt, Transformation first_transformation, Transformations... rest)
    {
        return Transform(first_transformation(std::move(pt)), rest...);
    }
}

#endif