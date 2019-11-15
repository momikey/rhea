#ifndef RHEA_GRAMMAR_OPERATORS_HPP
#define RHEA_GRAMMAR_OPERATORS_HPP

#include <tao/pegtl.hpp>

#include "tokens.hpp"
#include "keywords.hpp"
#include "strings.hpp"
#include "typenames.hpp"
#include "expression_fwd.hpp"

namespace rhea { namespace grammar {
    using namespace tao::pegtl;

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

    struct parenthesized : if_must <
        seq <
            one <'('>,
            pad < expression, ignored >
        >,
        one <')'>
    > {};

    struct expression_list : list_must <
        expression,
        one <','>,
        ignored
    > {};

    struct array_expression : if_must <
        one <'['>,
        pad <
            expression_list,
            ignored
        >,
        one <']'>
    > {};

    struct list_expression : if_must <
        seq <
            one <'('>,
            separator,
            // Lists can start out empty, because they're dynamic.
            // But they otherwise have to have at least one comma to tell them
            // apart from regular parentheses. (Like in Python.)
            sor <
                seq <
                    seq <expression, separator, one <','> >,
                    separator,
                    opt <expression_list>,
                    separator
                >,
                at < one <')'> >
            >
        >,
        one <')'>
    > {};

    struct tuple_expression : if_must <
        seq <
            one <'{'>,
            sor <
                pad <
                    expression_list,
                    ignored
                >,
                separator
            >
        >,
        one <'}'>
    > {};

    struct symbol_list_expression : if_must <
        string <'@', '{'>,
        pad <
            list <identifier, one <','>, ignored>,
            ignored
        >,
        one <'}'>
    > {};

    // A key-value pair is used in dictionaries and calling functions
    // with named arguements.
    template <typename Key>
    struct kv_pair : seq <
        Key,
        separator,
        one <':'>,
        separator,
        expression
    > {};

    struct dictionary_expression : if_must <
        seq <
            one <'{'>,
            separator,
            list_must <
                kv_pair <
                    sor <symbol_name, numeric_literal, string_literal>
                >,
                one <','>,
                ignored
            >,
            separator
        >,
        one <'}'>
    > {};

    struct operand : sor <
        array_expression,
        list_expression,
        dictionary_expression,
        tuple_expression,
        parenthesized,
        numeric_literal,
        string_literal,
        fully_qualified
    > {};

    ////
    /// New precedence levels go here
    ////

    struct subscript_expr : seq <
        one <'['>,
        pad < expression, ignored >,
        one <']'>
    > {};

    struct member_expr : seq <
        one <'.'>,
        separator,
        identifier
    > {};

    struct function_call_expr : seq <
        one <'('>,
        opt <
            pad <expression_list, ignored>
        >,
        one <')'>,
        opt <function_suffix>
    > {};

    struct postfix_op : seq <
        operand,
        star <
            pad <
                sor <
                    subscript_expr,
                    member_expr,
                    function_call_expr
                >,
                ignored
            >
        >
    > {};

    // Question: Should we split unary ops?
    // `not` probably should be at a precedence level similar to
    // the other Boolean operations. OTOH, `!` is at the same level
    // as other unary operators in C++.
    struct unary_prefix_op : sor <
        seq <
            sor <
                one <'^'>,
                one <'~'>,
                one <'*'>,
                seq < one <'+', '-'>, not_at <digit> >,
                seq <kw_not, spacer>
            >,
            postfix_op
        >,
        postfix_op
    > {};

    struct exponential_binop : right_associative <
        unary_prefix_op,
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
            kw_and,
            kw_or
        >
    > {};

    struct cast_op : sor <
        seq <
            boolean_binop,
            pad <kw_as, ignored>,
            type_name
        >,
        boolean_binop
    > {};

    struct ternary_op : sor <
        seq <
            kw_if,
            spacer,
            cast_op,
            separator,
            kw_then,
            spacer,
            cast_op,
            separator,
            kw_else,
            spacer,
            cast_op
        >,
        cast_op
    > {};

    struct expression : sor <
        ternary_op,
        operand,
        symbol_list_expression,
        symbol_name
    > {};

    // Certain statements require constant expressions. Technically,
    // we can't determine if an expression is constant during the
    // parsing phase, but we can throw a few of them out.
    struct constant_expression : sor <
        ternary_op,
        parenthesized,
        numeric_literal,
        string_literal,
        fully_qualified,
        symbol_name
    > {};

}}

#endif /* RHEA_GRAMMAR_OPERATORS_HPP */