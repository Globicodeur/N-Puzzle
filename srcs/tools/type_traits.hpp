#pragma once

namespace tools {

    template <class T>
    struct id {
        using type = boost::mpl::size_t<T::template type<1>::id>;
    };

    template <
        template <class...> class OutSeqType,
        class Sequence,
        std::size_t nSeqSize,
        class ... Elements
    > struct Unify {
        using Next = typename boost::mpl::deref<
            typename boost::mpl::max_element<
                Sequence,
                boost::mpl::less<
                    id<boost::mpl::_1>,
                    id<boost::mpl::_2>
                >
            >::type
        >::type;
        using type = typename Unify<
            OutSeqType,
            typename boost::mpl::erase_key<Sequence, Next>::type,
            nSeqSize - 1, Next, Elements...
        >::type;
    };

    template <
        template <class...> class OutSeqType,
        class Sequence,
        class ... Elements
    > struct Unify<OutSeqType, Sequence, 0ul, Elements...> {
        using type = OutSeqType<Elements...>;
    };

    template <HClass H> struct Wrapper {
        template <uint s> using type = H<s>;
    };

}
