#ifndef RHEA_AST_SELECTOR_HPP
#define RHEA_AST_SELECTOR_HPP

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

#include "../grammar.hpp"

#include "transform.hpp"

/*
 * PEGTL provides an automatic parse tree builder. It can also be used
 * to create an AST skeleton by adding a "selector": a templated struct
 * that tells the tree generator how to transform specific nodes.
 * 
 * For Rhea, we mostly want two types of nodes. First, concrete things
 * like integer or string literals. Second, abstract nodes representing
 * expressions, statements, etc.
 */

namespace rhea { namespace ast {
    using namespace tao::pegtl;
    using namespace rhea::grammar;

    /*
     * The base selector. For anything that PEGTL has built in, we can
     * use this. All our custom transformations will instead be specializations
     * of this template.
     */
    template <typename Rule>
    struct tree_selector : parse_tree::selector < Rule, 
        /*
         * parse_tree::store_content::on stores the matched content
         * into its node. This is the best choice for terminals that
         * need no further processing, apart from, e.g., conversion
         * from strings.
         */
        parse_tree::store_content::on <
            signed_integer,
            floating_point_number,
            hex_literal,
            boolean_literal,
            integer_literal_suffix,
            float_literal_suffix,
            character_string<'\''>,
            character_string<'"'>,
            identifier,
            function_suffix
            // and other rules that produce simple tokens

            // These are here temporarily (note prefixing commas)
            ,type_name
            // ,postfix_op
        >,

        /*
         * parse_tree::remove_content::on removes content (obviosuly);
         * we can use it for nodes such as operators, where we can
         * figure out what's going on with just the rule name. It's also
         * useful for certain type definitions that only contain other tokens.
         */
        parse_tree::remove_content::on <
            string_literal,
            integer_literal,
            float_literal,
            symbol_name,

            // Operators
            subscript_expr,
            member_expr,
            coerce_operator,
            bitnot_operator,
            dereference_operator,
            unary_plus_operator,
            unary_minus_operator,
            kw_not,
            kw_ref,
            kw_ptr,
            exponent_operator,
            multiply_operator,
            divide_operator,
            modulus_operator,
            add_operator,
            subtract_operator,
            left_shift_operator,
            right_shift_operator,
            equals_operator,
            not_equal_operator,
            greater_equal_operator,
            less_equal_operator,
            less_than_operator,
            greater_than_operator,
            bitand_operator,
            bitor_operator,
            bitxor_operator,
            kw_and,
            kw_or,
            kw_is,
            kw_as
        >,

        /*
         * parse_tree::fold_one::on removes its own content if it has multiple
         * children, or replaces itself with an only child. That's useful
         * for a lot of group/list/alternation things.
         */
        parse_tree::fold_one::on <
            numeric_literal,
            fully_qualified,
            postfix_expr

            // These are temorarily placed here for debugging
        >,

        /*
         * parse_tree::discard_empty:on stores the node if it has children,
         * discarding any of its own content; if no children, then it removes
         * the node entirely. This might come in handy in a few cases.
         */

        /*
         * Any custom selectors we want to create. These can transform a node
         * and its children in essentially any way, including removing some or
         * all of them. For the most part, we'll just want to make AST info
         * from them.
         */

        binop_rearrange::on <
            exponential_binop,
            multiplicative_binop,
            additive_binop,
            shift_binop,
            relation_binop,
            bitwise_binop,
            boolean_binop
        >,

        typecheck_rearrange::on <
            cast_op,
            type_check_op
        >,

        ternary_transform::on <
            ternary_op
        >,

        unary_rearrange::on <
            unary_prefix_op
        >,

        postfix_rearrange::on <
            postfix_op
        >
    > {};
}}

#endif /* RHEA_AST_SELECTOR_HPP */