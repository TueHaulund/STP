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
    class partial_query
    {
        public:
            typedef T value_type;

            template<typename InputIterator>
            partial_query(InputIterator p_begin, InputIterator p_end)
            {
                std::copy(p_begin, p_end, std::back_inserter(query_data));
                std::cout << "Constructor" << std::endl;
            }

            partial_query(const partial_query<T> &p_other)
            {
                query_data = p_other.query_data;
                std::cout << "Copy" << std::endl;
            }

            partial_query(partial_query<T>&& p_other)
            {
                query_data = std::move(p_other.query_data);
                std::cout << "Move" << std::endl;
            }

            std::vector<T>& data()
            {
                return query_data;
            }

        private:
            std::vector<T> query_data;
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

    template <typename> struct is_pq : std::false_type {};
    template <typename T> struct is_pq<partial_query<T>> : std::true_type {};

    template<typename T, typename Transformation, typename ...Transformations>
    struct return_type;

    template<typename T, typename Transformation>
    struct return_type<T, Transformation>
    {
        typedef typename std::result_of<Transformation(partial_query<T>)>::type type;
    };

    template<typename T, typename Transformation, typename ...Transformations>
    struct return_type
    {
        typedef typename std::result_of<Transformation(partial_query<T>)>::type transformed_type;
        typedef typename return_type<typename transformed_type::value_type, Transformations...>::type type;
    };

    template<typename T, typename Transformation>
    typename return_type<T, Transformation>::type Query(partial_query<T> pq, Transformation last_transformation)
    {
        return last_transformation(std::move(pq));
    }

    template <typename T, typename Transformation, typename... Transformations>
    typename return_type<T, Transformation, Transformations...>::type Query(partial_query<T> pq, Transformation first_transformation, Transformations... rest)
    {
        return Query(first_transformation(std::move(pq)), rest...);
    }
}

#endif