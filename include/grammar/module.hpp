#ifndef RHEA_GRAMMAR_MODULE_HPP
#define RHEA_GRAMMAR_MODULE_HPP

#include <tao/pegtl.hpp>

#include "keywords.hpp"
#include "tokens.hpp"
#include "statement.hpp"

namespace rhea { namespace grammar {
    using namespace tao::pegtl;

    // Top-level statements are those that define rather than executing code.
    // This includes most definitions/declarations, plus the module framework.
    struct top_level_statement : sor <
        function_definition,
        concept_definition,

        seq <
            sor <
                enum_declaration,
                structure_declaration,
                type_alias,
                constant_declaration,
                variable_declaration,
                use_statement,
                import_statement,
                export_statement
            >,
            separator,
            one <';'>
        >
    > {};

    // A module definition must start with a module statement,
    // which can be followed by any "top-level" statement.
    struct module_definition : pad <
        seq <
            module_statement,
            pad <one <';'>, ignored>,
            list <
                top_level_statement,
                separator
            >
        >,
        ignored
    > {};

    // A program isn't necessarily a module, though it is
    // always part of one. It must also include a main function,
    // but we won't check that here.
    struct program_definition : pad <
        list <
            top_level_statement,
            separator
        >,
        ignored
    > {};
}}

#endif /* RHEA_GRAMMAR_MODULE_HPP */