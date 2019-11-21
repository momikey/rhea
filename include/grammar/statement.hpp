#ifndef RHEA_GRAMMAR_STATEMENT_HPP
#define RHEA_GRAMMAR_STATEMENT_HPP

#include <tao/pegtl.hpp>

#include "keywords.hpp"
#include "tokens.hpp"
#include "typenames.hpp"
#include "expression.hpp"
#include "concepts.hpp"

namespace rhea { namespace grammar {
    using namespace tao::pegtl;

    // TODO: Add other types of assignment LHS, such as structure fields
    struct assignment_lhs : identifier
    {};

    struct assignment_rhs : sor <
        constructor_expression,
        expression
        // Other possibilities, like tuples, lists, etc.
    >
    {};

    struct assignment : seq <
        assignment_lhs,
        pad < one<'='>, ignored >,
        assignment_rhs
    > {};

    // The valid operators for compound assignment (like C's +=)
    struct augment_operators : sor <
        string <'*', '*'>,
        string <'<', '<'>,
        string <'>', '>'>,
        one <'+'>,
        one <'-'>,
        one <'*'>,
        one <'/'>,
        one <'%'>,
        one <'&'>,
        one <'|'>,
        one <'^'>
    > {};

    struct compound_assignment : seq <
        assignment_lhs,
        pad <
            seq < augment_operators, one <'='> >,
            ignored
        >,
        assignment_rhs
    > {};

    struct variable_initialization : seq <
        assignment_lhs,
        pad < one<'='>, ignored >,
        assignment_rhs
    > {};

    struct declaration_as_type : seq <
        assignment_lhs,
        pad < kw_as, ignored >,
        type_name
    > {};

    struct constant_declaration : seq <
        kw_const,
        spacer,
        variable_initialization
    > {};

    struct variable_declaration : seq <
        kw_var,
        spacer,
        // Variable declarations can have either an "as type" or an "= expr"
        sor <
            variable_initialization,
            declaration_as_type
        >
    > {};

    // Forward declaration
    struct statement;

    struct statement_block : if_must <
        seq <
            one <'{'>,
            separator,
            star <
                pad <statement, ignored>
            >,
            separator
        >,
        one <'}'>
    > {};

    // Most control-flow constrcuts can accept either a single statement
    // or a block of them.
    struct stmt_or_block : sor < statement_block, statement > {};

    struct else_block : opt <
        kw_else,
        spacer,
        stmt_or_block
    > {};

    struct if_statement : seq <
        kw_if,
        separator,
        expression,
        separator,
        stmt_or_block,
        separator,
        else_block
    > {};

    struct unless_statement : seq <
        kw_unless,
        separator,
        expression,
        separator,
        stmt_or_block
    > {};

    struct while_statement : seq <
        kw_while,
        separator,
        expression,
        separator,
        stmt_or_block
    > {};

    struct index_expression : seq <
        identifier,
        pad <kw_in, ignored>,
        expression
    > {};

    struct for_statement : seq <
        kw_for,
        separator,
        index_expression,
        separator,
        stmt_or_block
    > {};

    struct do_statement : seq <
        kw_do,
        separator,
        expression
    > {};

    struct with_statement : seq <
        kw_with,
        separator,
        if_must_else <
            one <'('>,
            seq <
                pad <declaration_as_type, ignored>,
                one <')'>
            >,
            declaration_as_type
        >,
        separator,
        stmt_or_block
    > {};

    // A single case in a match-on statement
    struct on_case : seq <
        kw_on,
        separator,
        constant_expression,
        pad <one <':'>, ignored>,
        stmt_or_block
    > {};

    // A call to a predicate function, with optional arguments
    struct predicate_call : seq <
        fully_qualified,
        separator,
        opt_must <
            seq <
                one <'('>,
                opt <
                    pad <expression_list, ignored>
                >
            >,
            one <')'>
        >,
        separator,
        one <'?'>
    > {};

    // A single case in a match-when statement
    struct when_case : seq <
        kw_when,
        separator,
        predicate_call,
        pad <one <':'>, ignored>,
        stmt_or_block
    > {};

    // The default case for match-on or match-when
    struct default_case : seq <
        kw_default,
        pad <one <':'>, ignored>,
        stmt_or_block
    > {};

    // A case in a match-type statement
    struct type_case : seq <
        type_assertion,
        pad <one <':'>, ignored>,
        stmt_or_block
    > {};

    template <typename M>
    struct match_block : seq <
        one <'{'>,
        separator,
        opt <
            list_tail <M, separator>
        >,
        opt <default_case>,
        separator,
        one <'}'>
    > {};

    struct match_on_statement : seq <
        kw_match,
        separator,
        fully_qualified,
        separator,
        match_block <on_case>
    > {};

    struct match_when_statement : seq <
        kw_match,
        separator,
        fully_qualified,
        separator,
        match_block <when_case>
    > {};

    struct match_type_statement : seq <
        kw_match,
        separator,
        fully_qualified,
        separator,
        match_block <type_case>
    > {};

    struct enum_declaration : seq <
        type_definition_lhs,
        separator,
        symbol_list_expression
    > {};

    struct structure_declaration : seq <
        type_definition_lhs,
        separator,
        if_must <
            seq <
                one <'{'>,
                separator,
                opt <
                    list <type_pair, one <','>, ignored>
                >,
                separator
            >,
            one <'}'>
        >
    > {};

    struct type_alias : seq <
        type_definition_lhs,
        separator,
        type_name
    > {};

    struct throw_statement : seq <
        kw_throw,
        separator,
        expression
    > {};

    struct catch_statement : seq <
        kw_catch,
        separator,
        if_must <
            seq <
                one <'{'>,
                pad <type_pair, ignored>
            >,
            one <'}'>
        >,
        separator,
        stmt_or_block
    > {};

    struct finally_statement : seq <
        kw_finally,
        separator,
        stmt_or_block
    > {};

    // The "head" of a try statement is the try block, which is also
    // the only required part.
    struct try_statement_head : seq <
        kw_try,
        separator,
        stmt_or_block
    > {};

    struct try_statement : seq <
        try_statement_head,
        separator,
        opt <
            list <catch_statement, ignored>
        >,
        separator,
        opt <finally_statement>
    > {};

    ////
    // Function definitions
    ////
    
    // All the parts come first, then we build the definition rule
    // out of them at the end.

    // Helper template to reduce redundancy
    template <typename Expr>
    struct fn_values_list : if_must <
        seq <
            one <'{'>,
            separator,
            list <Expr, one <','>, ignored>,
            separator
        >,
        one <'}'>
    > {};

    // If the wildcard argument is used, no others are allowed.
    struct wildcard_argument : seq <
        one <'{'>,
        pad <
            seq <
                identifier,
                pad <one <':'>, ignored>,
                one <'*'>
            >,
            ignored
        >,
        one <'}'>
    > {};

    struct arguments_list : sor <
        fn_values_list < sor <type_match, concept_match> >,
        wildcard_argument
    > {};

    struct predicate_arguments_list : if_must <
        seq <
            one <'('>,
            separator,
            list <expression, one <','>, ignored>,
            separator
        >,
        one <')'>
    > {};

    // Predicate calls are used in preconditions and postconditions.
    struct fn_condition_predicate_call : seq <
        identifier,
        pad <one <'.'>, ignored>,
        identifier,
        pad <opt <predicate_arguments_list>, ignored>,
        separator,
        one <'?'>
    > {};

    struct conditions_list : fn_values_list <fn_condition_predicate_call> {};

    // The "with" block indicates pre/postconditions.
    struct fn_with_block : seq <
        kw_with,
        separator,
        conditions_list
    > {};

    // Return statements can only come insside functions, though there's
    // nothing in the parser that ensures this.
    struct return_statement : seq <
        kw_return,
        separator,
        expression
    > {};

    // Return types are enclosed in square brackets, and are optional.
    struct return_type : if_must <
        one <'['>,
        pad <type_name, ignored>,
        one <']'>        
    > {};

    // Different kinds of functions. We use different rules to match them.
    struct function_name : identifier {};
    struct predicate_name : seq < function_name, separator, one <'?'> > {};
    struct operator_name : seq < function_name, separator, one <'$'> > {};
    struct unchecked_name : seq < function_name, separator, one <'!'> > {};

    // The generic parts of a function.
    template <typename Expr>
    struct generic_part : seq <
        one < '<' >,
        pad <Expr, ignored>,
        one < '>' >
    > {};

    struct generic_concept : generic_part <concept_match> {};
    struct generic_specialization : generic_part <type_match> {};
    struct generic_function_type : sor <generic_concept, generic_specialization> {};

    // Separate rules for different functions.

    // Basic function definition, with all optionas possible
    struct basic_function_def : seq <
        kw_def,
        separator,
        function_name,
        separator,
        opt < pad <generic_function_type, ignored> >,
        opt < pad <return_type, ignored> >,
        opt < pad <arguments_list, ignored> >,
        opt < pad <fn_with_block, ignored> >,
        pad < one <'='>, ignored >,
        stmt_or_block
    > {};
    
    // Unchecked can't have pre/postconditions
    struct unchecked_function_def : seq <
        kw_def,
        separator,
        unchecked_name,
        separator,
        opt < pad <generic_function_type, ignored> >,
        opt < pad <return_type, ignored> >,
        opt < pad <arguments_list, ignored> >,
        pad < one <'='>, ignored >,
        stmt_or_block
    > {};

    // Predicates have fixed return types
    struct predicate_function_def : seq <
        kw_def,
        separator,
        predicate_name,
        separator,
        opt < pad <generic_function_type, ignored> >,
        opt < pad <arguments_list, ignored> >,
        opt < pad <fn_with_block, ignored> >,
        pad < one <'='>, ignored >,
        stmt_or_block
    > {};

    // Operator/conversion functions don't have special syntax (yet)
    struct operator_function_def : seq <
        kw_def,
        separator,
        operator_name,
        separator,
        opt < pad <generic_function_type, ignored> >,
        opt < pad <return_type, ignored> >,
        opt < pad <arguments_list, ignored> >,
        opt < pad <fn_with_block, ignored> >,
        pad < one <'='>, ignored >,
        stmt_or_block
    > {};

    // Now put them all together, but basic last (because of suffixes).
    struct function_definition : sor <
        predicate_function_def,
        operator_function_def,
        unchecked_function_def,
        basic_function_def
    > {};

    ////
    // Concepts
    ////

    struct concept_definition : seq <
        kw_concept,
        separator,
        identifier,
        separator,
        concept_generic_type,
        pad <one <'='>, ignored>,
        concept_block
    > {};

    ////
    // Modules
    ////

    struct relative_module_name : seq <
        one <':'>,
        separator,
        fully_qualified
    > {};

    struct module_name : sor <fully_qualified, relative_module_name> {};

    struct module_statement : seq <
        kw_module,
        separator,
        module_name
    > {};

    struct use_statement : seq <
        kw_use,
        separator,
        module_name
    > {};

    struct import_list : seq <
        one <'{'>,
        separator,
        list <identifier, one <','>, ignored>,
        separator,
        one <'}'>
    > {};

    struct import_statement : seq <
        kw_import,
        pad <import_list, ignored>,
        kw_from,
        separator,
        module_name
    > {};

    struct export_statement : seq <
        kw_export,
        pad <import_list, ignored>
    > {};

    ////
    // Statement
    ////
    struct statement : sor <
        // These are "block" statements
        if_statement,
        unless_statement,
        while_statement,
        for_statement,
        with_statement,
        match_on_statement,
        match_when_statement,
        match_type_statement,
        function_definition,
        concept_definition,

        // These all end in a semicolon
        seq <
            sor <
                enum_declaration,
                structure_declaration,
                type_alias,
                constant_declaration,
                variable_declaration,
                do_statement,
                assignment,
                compound_assignment,
                kw_break,
                kw_continue,
                try_statement,
                throw_statement,
                return_statement,
                module_statement,
                use_statement,
                import_statement,
                export_statement,

                // If all else fails, try a bare expression
                expression
            >,
            separator,
            one <';'>
        >
    > {};

}}

#endif /* RHEA_GRAMMAR_STATEMENT_HPP */