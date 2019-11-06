#ifndef RHEA_GRAMMAR_OPERATORS_HPP
#define RHEA_GRAMMAR_OPERATORS_HPP

#include <tao/pegtl.hpp>

#include "tokens.hpp"

namespace rhea { namespace grammar {
    using namespace tao::pegtl;

    struct expression;

    // Left and right associativity helpers
    // (Taken from the PEGTL Lua example)
    template <typename S, typename Op>
    struct left_associative : seq <
        S,
        separator,
        star_must <Op, separator, S, separator>
    > {};

    template <typename S, typename Op>
    struct right_associative : seq <
        S,
        separator,
        opt_must <
            Op,
            separator,
            right_associative<S,Op>
        >
    > {};

    struct literal : sor <
        hex_literal,
        float_literal,
        signed_integer,
        seq <unsigned_integer, one <'u'> >
    > {};

    struct parenthesized : if_must <
        one <'('>,
        pad < expression, ignored >,
        one <')'>
    > {};

    struct base_exp : sor <
        parenthesized,
        literal,
        identifier
    > {};

    ////
    /// New precedence levels go here
    ////

    // struct cast_op : seq <
    //     sor <base_exp, exponential_binop>,
    //     pad <TAO_PEGTL_KEYWORD("as"), ignored>,
    //     type_name
    // > {};

    // struct unary_prefix_op : seq <
    //     sor <
    //         one <'^'>,
    //         one <'~'>,
    //         seq < one <'+', '-'>, not_at <digit> >,
    //         TAO_PEGTL_KEYWORD("not")
    //     >,
    //     cast_op
    // > {};

    struct exponential_binop : right_associative <
        base_exp,
        TAO_PEGTL_STRING("**")
    > {};

    struct multiplicative_binop : left_associative <
        exponential_binop,
        sor <
            seq< one <'*'>, not_at <one <'*'>>>,
            one <'/'>,
            one <'%'>
        >        
    > {};

    struct additive_binop : left_associative <
        multiplicative_binop,
        sor <
            one <'+'>,
            one <'-'>
        >        
    > {};

    struct shift_binop : left_associative <
        additive_binop,
        sor <
            TAO_PEGTL_STRING("<<"),
            TAO_PEGTL_STRING(">>")
        >
    > {};

    struct relation_binop : left_associative <
        shift_binop,
        sor <
            TAO_PEGTL_STRING("=="),
            TAO_PEGTL_STRING("!="),
            TAO_PEGTL_STRING("<="),
            TAO_PEGTL_STRING(">="),

            // We have to use lookahead here so that these won't
            // get picked up ahead of the shift operators below.
            seq < one <'<'>, not_at < one <'<'> > >,
            seq < one <'>'>, not_at < one <'>'> > >
        >
    > {};

    struct bitwise_binop : left_associative <
        relation_binop,
        sor <
            one <'&'>,
            one <'|'>,
            one <'^'>
        >
    > {};

    struct boolean_binop : left_associative <
        bitwise_binop,
        sor <
            TAO_PEGTL_KEYWORD("and"),
            TAO_PEGTL_KEYWORD("or")
        >
    > {};

    struct expression : sor <
        boolean_binop,
        base_exp
    > {};
}}

#endif /* RHEA_GRAMMAR_OPERATORS_HPP */