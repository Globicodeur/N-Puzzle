#pragma once

namespace tools {

    template <
        class In,
        template <class...> class Out,
        class Compare,
        std::size_t size,
        class... Elements
    >
    struct Sort {

        using Next = typename boost::mpl::deref<
            typename boost::mpl::max_element<
                In,
                Compare
            >::type
        >::type;

        using type = typename Sort<
            typename boost::mpl::erase_key<In, Next>::type,
            Out,
            Compare,
            size - 1,
            Next, Elements...
        >::type;

    };

    template <
        class In,
        template <class...> class Out,
        class Compare,
        class... Elements
    >
    struct Sort<In, Out, Compare, 0ul, Elements...> {

        using type = Out<Elements...>;

    };

}
