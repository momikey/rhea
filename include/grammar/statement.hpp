#ifndef RHEA_GRAMMAR_STATEMENT_HPP
#define RHEA_GRAMMAR_STATEMENT_HPP

#include "keywords.hpp"
#include "tokens.hpp"
#include "typenames.hpp"
#include "operators.hpp"

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
    struct match_block : if_must <
        seq <
            one <'{'>,
            separator,
            opt <
                list_tail <M, separator>
            >,
            opt <default_case>,
            separator
        >,
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

    // Match any kind of statement
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

                // If all else fails, try a bare expression
                expression
            >,
            separator,
            one <';'>
        >
    > {};

}}

#endif /* RHEA_GRAMMAR_STATEMENT_HPP */