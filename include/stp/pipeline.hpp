#ifndef STP_PIPELINE_HPP
#define STP_PIPELINE_HPP

#include <type_traits>

namespace stp
{
    namespace detail
    {
        template
        <
            typename OpType,
            typename ...Rest
        >
        class stp_type : public stp_type<Rest...>
        {
            private:
                using base_type = stp_type<Rest...>;
                using this_type = stp_type<OpType, Rest...>;

                template <typename SequenceType>
                using op_return_type = typename std::result_of<OpType(SequenceType)>::type;

            public:
                template <typename SequenceType>
                using return_type = typename base_type::template return_type<op_return_type<SequenceType>>;

                stp_type(OpType operation, Rest... rest) : base_type(rest...), operation_(operation)
                {}

                template <typename SequenceType>
                return_type<SequenceType> operator()(SequenceType sequence) const
                {
                    using base_return = typename std::result_of<base_type(op_return_type<SequenceType>)>::type;
                    using this_return = typename std::result_of<this_type(SequenceType)>::type;
                    static_assert(std::is_same<base_return, this_return>::value, "Pipeline type mismatch");

                    return base_type::operator()(operation_(sequence));
                }

            private:
                OpType operation_;
        };

        template <typename OpType>
        class stp_type<OpType>
        {
            public:
                template <typename SequenceType>
                using return_type = typename std::result_of<OpType(SequenceType)>::type;

                stp_type(OpType operation) : operation_(operation)
                {}

                template <typename SequenceType>
                return_type<SequenceType> operator()(SequenceType sequence) const
                {
                    return operation_(sequence);
                }

            private:
                OpType operation_;
        };
    }

    template
    <
        typename OpType,
        typename ...Rest
    >
    detail::stp_type<OpType, Rest...> make_stp(OpType operation, Rest... rest)
    {
        return detail::stp_type<OpType, Rest...>(operation, rest...);
    }
}

#endif