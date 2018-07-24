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

    template <class N, N n, N up_to, class F>
    constexpr auto reify_range_impl(N rn, F f) {
        if constexpr (n > up_to) {
            (void)rn; (void)f;
            throw std::out_of_range { "value to reify is not in the range" };
        }
        else {
            if (n == rn) return f(hana::integral_c<N, n>);
            else         return reify_range_impl<N, n + 1, up_to>(rn, f);
        }
    }

    template <auto from, auto up_to, class N>
    constexpr auto reify_range(N n) {
        return [=](auto f) {
            return reify_range_impl<N, from, up_to>(n, f);
        };
    }

}
