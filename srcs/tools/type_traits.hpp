#pragma once

namespace tools {

    template <
        template <class...> class OutSeqType,
        class Sequence,
        std::size_t nSeqSize,
        class ... Elements
    > struct Unify {
        using Next = typename boost::mpl::front<Sequence>::type;
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

    template <HClass H> struct Wrapper { };

}
