#ifndef RHEA_GRAMMAR_TOKENS_HPP
#define RHEA_GRAMMAR_TOKENS_HPP

#include <tao/pegtl.hpp>

namespace rhea { namespace grammar {
    using namespace tao::pegtl;

    struct line_comment : seq <
        one < '#' >,
        until<eolf>
    > {};

    struct block_comment : seq <
        string < '#', '{' >,
        must <
            until <
                string < '#', '}' >
            >
        >
    > {};

    // A single ignored token can be useful if we use pad functions
    struct ignored : sor <space, line_comment, block_comment> {};
    struct separator : star<ignored> {};

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