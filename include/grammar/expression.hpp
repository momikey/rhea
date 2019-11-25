#ifndef RHEA_GRAMMAR_EXPRESSION_HPP
#define RHEA_GRAMMAR_EXPRESSION_HPP

#include <tao/pegtl.hpp>

#include "tokens.hpp"
#include "keywords.hpp"
#include "strings.hpp"
#include "typenames.hpp"
#include "operator.hpp"
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

    struct dictionary_entry : kv_pair <
        sor <symbol_name, numeric_literal, string_literal>
    > {};

    struct dictionary_expression : if_must <
        seq <
            one <'{'>,
            separator,
            list_must <
                dictionary_entry,
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
        dot_operator,
        separator,
        identifier
    > {};

    struct named_argument : kv_pair <identifier> {};

    struct named_argument_list : list_must <
        named_argument,
        one <','>,
        ignored
    > {};

    struct unnamed_argument_list : expression_list {};

    struct empty_argument_list : success {};

    struct function_argument_list : pad <
        sor <named_argument_list, unnamed_argument_list, empty_argument_list>,
        ignored
    > {};

    struct function_call_expr : seq <
        one <'('>,
        pad <
            sor <named_argument_list, unnamed_argument_list, empty_argument_list>,
            ignored
        >,
        one <')'>,
        opt <function_suffix>
    > {};

    struct postfix_expr : sor <
        subscript_expr,
        member_expr,
        function_call_expr
    > {};

    struct postfix_op : seq <
        operand,
        star <
            pad <postfix_expr, ignored>
        >
    > {};

    // Question: Should we split unary ops?
    // `not` probably should be at a precedence level similar to
    // the other Boolean operations. OTOH, `!` is at the same level
    // as other unary operators in C++.
    struct unary_prefix_op : sor <
        seq <
            sor <
                coerce_operator,
                bitnot_operator,
                dereference_operator,
                seq < 
                    sor <unary_plus_operator, unary_minus_operator>,
                    not_at <digit>
                >,
                seq <kw_not, separator>,
                seq <kw_ref, separator>,
                seq <kw_ptr, separator>
            >,
            unary_prefix_op
        >,
        postfix_op
    > {};

    struct exponential_binop : right_associative <
        unary_prefix_op,
        exponent_operator
    > {};

    struct multiplicative_binop : left_associative <
        exponential_binop,
        sor <multiply_operator, divide_operator, modulus_operator>
    > {};

    struct additive_binop : left_associative <
        multiplicative_binop,
        sor <add_operator, subtract_operator>        
    > {};

    struct shift_binop : left_associative <
        additive_binop,
        sor <left_shift_operator, right_shift_operator>
    > {};

    struct relation_binop : left_associative <
        shift_binop,
        sor <
            equals_operator,
            not_equal_operator,
            greater_equal_operator,
            less_equal_operator,
            less_than_operator,
            greater_than_operator
        >
    > {};

    struct bitwise_binop : left_associative <
        relation_binop,
        sor <
            bitand_operator,
            bitor_operator,
            bitxor_operator
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
            separator,
            kw_as,
            separator,
            type_name
        >,
        boolean_binop
    > {};

    struct type_check_op : sor <
        seq <
            cast_op,
            separator,
            kw_is,
            separator,
            type_name
        >,
        cast_op
    > {};

    struct ternary_op : sor <
        seq <
            kw_if,
            spacer,
            type_check_op,
            separator,
            kw_then,
            spacer,
            type_check_op,
            separator,
            kw_else,
            spacer,
            type_check_op
        >,
        type_check_op
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

#endif /* RHEA_GRAMMAR_EXPRESSION_HPP */