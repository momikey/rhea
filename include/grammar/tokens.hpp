#ifndef RHEA_GRAMMAR_TOKENS_HPP
#define RHEA_GRAMMAR_TOKENS_HPP

#include <tao/pegtl.hpp>

#include "keywords.hpp"

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

    // A separator is for tokens that don't require space (such as operators)
    struct separator : star<ignored> {};

    // A spacer is for tokens that *do* need space, like keywords
    struct spacer : plus<ignored> {};

    struct unsigned_integer : rep_min_max <1, 20, digit> {};

    struct signed_integer : seq <
        opt < one < '+', '-' > >,
        rep_min_max <1, 20, digit>
    > {};

    struct integer_literal_suffix : seq <
        one <'_'>,
        sor <
            istring <'u', 'b'>,
            istring <'u', 'l'>,
            istring <'u'>,
            istring <'b'>,
            istring <'l'>
        >
    > {};

    struct integer_literal : seq <
        signed_integer,
        opt <integer_literal_suffix>
    > {};

    // Note: We only have one possible float suffix at the moment,
    // but we still make it a separate token. We might want to add
    // more later on (e.g., hex floats or arbitrary-precision), and
    // it's easier to manipulate the AST if we have named rules.
    struct float_literal_suffix : seq <
        one <'_'>,
        istring <'f'>
    > {};

    struct hex_literal : seq <
        string < '0', 'x' >,
        must < rep_min_max <1, 16, xdigit> >
    > {};

    struct float_literal : seq <
        opt < one<'+', '-'> >,
        plus <digit>,
        opt <
            one <'.'>,
            star <digit>
        >,
        opt_must <
            one <'e','E'>,
            signed_integer
        >,
        opt <float_literal_suffix>
    > {};

    struct boolean_literal : sor <kw_true, kw_false> {};

    struct literal : sor <
        boolean_literal,
        hex_literal,
        float_literal,
        integer_literal
    > {};

    // TODO: Detect reserved words and raise errors
    struct identifier : seq <
        identifier_first,
        star <identifier_other>
    > {};

    // TODO: Add in other type qualifiers
    struct type_name : identifier {};

    struct symbol_name : if_must <
        one <'@'>,
        identifier
    > {};
}}

#endif /* RHEA_GRAMMAR_TOKENS_HPP */