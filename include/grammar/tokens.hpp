#ifndef RHEA_GRAMMAR_TOKENS_HPP
#define RHEA_GRAMMAR_TOKENS_HPP

#include <tao/pegtl.hpp>

namespace rhea { namespace grammar {
    using namespace tao::pegtl;

    struct spacing : star <space> {};

    struct line_comment : seq <
        one < '#' >,
        until<eolf>
    > {};

    struct block_comment : must <
        string < '#', '{' >,
        until <
            string < '#', '}' >
        >
    > {};

    struct unsigned_integer : rep_min_max <1, 20, digit> {};

    struct signed_integer : seq <
        opt < one < '+', '-' > >,
        rep_min_max <1, 20, digit>
    > {};

    struct identifier : seq <
        identifier_first,
        star <identifier_other>
    > {};
}}

#endif /* RHEA_GRAMMAR_TOKENS_HPP */