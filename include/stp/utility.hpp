#ifndef STP_UTILITY_HPP
#define STP_UTILITY_HPP

#include <type_traits>

namespace stp
{
    namespace detail
    {
        namespace utility
        {
            template
            <
                bool PBR,
                bool RBR,
                typename FunctorType,
                typename ReturnType,
                typename ParameterType
            >
            struct functor_traits_base
            {
                static const bool pass_by_reference = PBR;
                static const bool return_by_reference = RBR;

                using parameter_type = typename std::conditional<pass_by_reference, typename std::add_lvalue_reference<ParameterType>::type, ParameterType>::type;
                using return_type = typename std::conditional<return_by_reference, typename std::add_lvalue_reference<ReturnType>::type, ReturnType>::type;
                using functor_type = FunctorType;

                //Sanity Check
                static_assert(std::is_same<return_type, typename std::result_of<FunctorType(parameter_type)>::type>::value, "Functor return type mismatch");
            };

            template <typename>
            struct functor_traits_pm;

            template
            <
                typename FunctorType,
                typename ReturnType,
                typename ParameterType
            >
            struct functor_traits_pm <ReturnType (FunctorType::*)(ParameterType) const> : public functor_traits_base<false, false, FunctorType, ReturnType, ParameterType>
            {};

            template
            <
                typename FunctorType,
                typename ReturnType,
                typename ParameterType
            >
            struct functor_traits_pm <ReturnType& (FunctorType::*)(ParameterType&) const> : public functor_traits_base<true, true, FunctorType, ReturnType, ParameterType>
            {};

            template
            <
                typename FunctorType,
                typename ReturnType,
                typename ParameterType
            >
            struct functor_traits_pm <ReturnType (FunctorType::*)(ParameterType&) const> : public functor_traits_base<true, false, FunctorType, ReturnType, ParameterType>
            {};

            template
            <
                typename FunctorType,
                typename ReturnType,
                typename ParameterType
            >
            struct functor_traits_pm <ReturnType& (FunctorType::*)(ParameterType) const> : public functor_traits_base<false, true, FunctorType, ReturnType, ParameterType>
            {};

            template
            <
                typename FunctorType,
                typename SequenceType
            >
            struct functor_traits : public functor_traits_pm<decltype(&FunctorType::template operator()<typename std::decay<SequenceType>::type>)>
            {};
        }
    }
}

#endif