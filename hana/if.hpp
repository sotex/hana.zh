/*!
@file
Defines `boost::hana::if_`.

@copyright Louis Dionne 2013-2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_IF_HPP
#define BOOST_HANA_IF_HPP

#include <hana/fwd/if.hpp>

#include <hana/concept/logical.hpp>
#include <hana/config.hpp>
#include <hana/core/dispatch.hpp>
#include <hana/eval_if.hpp>


BOOST_HANA_NAMESPACE_BEGIN
    //! @cond
    template <typename Cond, typename Then, typename Else>
    constexpr decltype(auto) if_t::operator()(Cond&& cond, Then&& then, Else&& else_) const {
        using Bool = typename hana::tag_of<Cond>::type;
        using If = BOOST_HANA_DISPATCH_IF(if_impl<Bool>,
            hana::Logical<Bool>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(hana::Logical<Bool>::value,
        "hana::if_(cond, then, else) requires 'cond' to be a Logical");
    #endif

        return If::apply(static_cast<Cond&&>(cond),
                         static_cast<Then&&>(then),
                         static_cast<Else&&>(else_));
    }
    //! @endcond

    namespace detail {
        template <typename T>
        struct hold {
            T value;
            constexpr T&& operator()() && { return static_cast<T&&>(value); }
        };
    }

    template <typename L, bool condition>
    struct if_impl<L, when<condition>> : default_ {
        template <typename C, typename T, typename E>
        static constexpr auto apply(C&& c, T&& t, E&& e) {
            return hana::eval_if(static_cast<C&&>(c),
                detail::hold<T&&>{static_cast<T&&>(t)},
                detail::hold<E&&>{static_cast<E&&>(e)}
            );
        }
    };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_IF_HPP
