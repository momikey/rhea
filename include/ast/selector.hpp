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
            builtin_types,
            identifier,
            function_name,
            function_suffix
            // and other rules that produce simple tokens
        >,

        /*
         * parse_tree::remove_content::on removes content (obviosuly);
         * we can use it for nodes such as operators, where we can
         * figure out what's going on with just the rule name. It's also
         * useful for certain type definitions that only contain other tokens,
         * so most of our non-terminals will actually use this selector if
         * they don't need to do further rearranging.
         */
        parse_tree::remove_content::on <
            string_literal,
            integer_literal,
            float_literal,
            symbol_name,
            nothing_literal,
            named_argument_list,
            unnamed_argument_list,
            empty_argument_list,
            dictionary_entry,
            dictionary_expression,
            tuple_expression,
            list_expression,
            array_expression,
            symbol_list_expression,
            relative_identifier,

            // Operators
            function_call_expr,
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
            kw_as,

            // Statements
            assignment,
            compound_assignment,
            bare_expression,
            declaration_as_type,
            constant_declaration,
            statement_block,
            if_statement,
            unless_statement,
            while_statement,
            for_statement,
            do_statement,
            kw_break,
            kw_continue,
            with_statement,
            with_declaration,
            on_case,
            predicate_call,
            when_case,
            default_case,
            type_case,
            match_on_statement,
            match_when_statement,
            match_type_statement,
            enum_declaration,
            structure_declaration,
            type_alias,
            try_statement,
            catch_statement,
            throw_statement,
            finally_statement,

            // Functions
            arguments_list,
            wildcard_argument,
            predicate_arguments_list,
            fn_condition_predicate_call,
            fn_with_block,
            return_statement,
            return_type,
            generic_function_type,
            basic_function_def,
            predicate_function_def,
            unchecked_function_def,
            operator_function_def,
            extern_declaration,

            // Typenames
            generic_type,
            array_type,
            return_type_spec,
            // simple_type_name,
            complex_type_name,
            variant_type_list,
            optional_type,
            type_pair,
            constructor_expression,
            type_assertion,

            // Concepts
            concept_match,
            concept_function_name,
            concept_function_type,
            concept_function_check,
            concept_member_check,
            concept_block,
            concept_definition,

            // Modules
            module_statement,
            use_statement,
            import_list,
            import_statement,
            export_statement,
            module_name,

            // Top-level nodes
            module_definition,
            program_definition
        >,

        /*
         * parse_tree::fold_one::on removes its own content if it has multiple
         * children, or replaces itself with an only child. That's useful
         * for a lot of group/list/alternation things.
         */
        parse_tree::fold_one::on <
            numeric_literal,
            fully_qualified,
            any_identifier,
            postfix_expr,
            named_argument,
            function_argument_list,
            type_name,
            constant_expression,
            type_match,
            tagged_union,
            concept_generic_type,
            assignment_rhs,
            augment_operators,
            variable_declaration,
            exception_spec,
            statement,
            stmt_or_block,
            predicate_name,
            operator_name,
            unchecked_name,
            function_definition,
            simple_type_name,
            concept_check,
            top_level_statement

            // These are temorarily placed here for debugging
        >,

        /*
         * parse_tree::discard_empty:on stores the node if it has children,
         * discarding any of its own content; if no children, then it removes
         * the node entirely. This might come in handy in a few cases.
         */
        // parse_tree::discard_empty::on <
        // >,

        /*
         * Any custom selectors we want to create. These can transform a node
         * and its children in essentially any way, including removing some or
         * all of them. For the most part, we'll just want to make AST info
         * from them.
         */

        discard_subtree::on <
            complex_type_lookahead,
            type_declaration_operator
        >,

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
            unary_prefix_op,
            boolean_not_op,
            pointer_or_reference_name,
            assignment_lhs,
            either_type_name
        >,

        postfix_rearrange::on <
            postfix_op
        >,

        else_rearrange::on <
            else_block
        >
    > {};
}}

#endif /* RHEA_AST_SELECTOR_HPP */