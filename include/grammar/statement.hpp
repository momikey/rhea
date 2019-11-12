#ifndef RHEA_GRAMMAR_STATEMENT_HPP
#define RHEA_GRAMMAR_STATEMENT_HPP

#include "keywords.hpp"
#include "tokens.hpp"
#include "operators.hpp"

namespace rhea { namespace grammar {
    using namespace tao::pegtl;

    // TODO: Add other types of assignment LHS, such as structure fields
    struct assignment_lhs : identifier
    {};

    struct assignment_rhs : sor <
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

    struct constant_declaration : seq <
        kw_const,
        spacer,
        identifier,
        pad < one <'='>, ignored >,
        expression
    > {};

    struct variable_declaration : seq <
        kw_var,
        spacer,
        identifier,
        // Variable declarations can have either an "as type" or an "= expr"
        sor <
            seq <
                pad < one <'='>, ignored >,
                expression
            >,
            seq <
                spacer,
                kw_as,
                spacer,
                type_name
            >
        >
    > {};

    struct statement : seq <
        // Add all possible statements here
        sor <
            constant_declaration,
            variable_declaration,
            assignment,
            compound_assignment
        >,
        separator,
        one <';'>
    > {};

    struct statement_block : if_must <
        one <'{'>,
        separator,
        star <
            pad <statement, ignored>
        >,
        separator,
        one <'}'>
    > {};
}}

#endif /* RHEA_GRAMMAR_STATEMENT_HPP */