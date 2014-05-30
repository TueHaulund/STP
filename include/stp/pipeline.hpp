#ifndef STP_PIPELINE_HPP
#define STP_PIPELINE_HPP

#include <type_traits>

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
            private:
                using base_type = stp_type<Rest...>;
                using this_type = stp_type<TransformationType, Rest...>;

                template <typename SequenceType>
                using transformed_type = typename std::result_of<TransformationType(SequenceType)>::type;

            public:
                template <typename SequenceType>
                using return_type = typename base_type::template return_type<transformed_type<SequenceType>>;

                stp_type(TransformationType transform, Rest... rest) : base_type(rest...), transform_(transform)
                {}

                template <typename SequenceType>
                return_type<SequenceType> operator()(SequenceType sequence) const
                {
                    using base_return = typename std::result_of<base_type(transformed_type<SequenceType>)>::type;
                    using this_return = typename std::result_of<this_type(SequenceType)>::type;
                    static_assert(std::is_same<base_return, this_return>::value, "Pipeline type mismatch");

                    return base_type::operator()(transform_(sequence));
                }

            private:
                TransformationType transform_;
        };

        template <typename TransformationType>
        class stp_type<TransformationType>
        {
            public:
                template <typename SequenceType>
                using return_type = typename std::result_of<TransformationType(SequenceType)>::type;

                stp_type(TransformationType transform) : transform_(transform)
                {}

                template <typename SequenceType>
                return_type<SequenceType> operator()(SequenceType sequence) const
                {
                    return transform_(sequence);
                }

            private:
                TransformationType transform_;
        };
    }

    template
    <
        typename TransformationType,
        typename ...Rest
    >
    detail::stp_type<TransformationType, Rest...> make_stp(TransformationType transform, Rest... rest)
    {
        return detail::stp_type<TransformationType, Rest...>(transform, rest...);
    }
}

#endif