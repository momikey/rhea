#ifndef RHEA_GRAMMAR_CONCEPTS_HPP
#define RHEA_GRAMMAR_CONCEPTS_HPP

#include <tao/pegtl.hpp>

#include "tokens.hpp"
#include "typenames.hpp"
#include "operators.hpp"

namespace rhea { namespace grammar {
    using namespace tao::pegtl;

    // A concept match in a function argument or generic type.
    struct concept_match : seq <
        identifier,
        pad < string <'~', '>'>, ignored >,
        type_name
    > {};

    // A generic type in a concept check.
    struct concept_generic_type : seq <
        one <'<'>,
        pad <identifier, ignored>,
        one <'>'>
    > {};

    // A function name in a concept check.
    struct concept_function_name : seq <
        fully_qualified,
        opt <function_suffix>
    > {};

    // The type of a generic function, as checked in a concept.
    // Essentially, this parses a generic function (ignoring arguments)
    // that returns a specific type.
    struct concept_function_type : seq <
        concept_function_name,
        pad <concept_generic_type, ignored>,
        pad <return_type_spec, ignored>
    > {};

    // A concept function check: given a type T, is the function F
    // returning type R defined for that type? In Rhea syntax: `T => F <T> -> R`
    struct concept_function_check : seq <
        identifier,
        pad <string <'=', '>'>, ignored>,
        concept_function_type
    > {};

    // A concept member check: given a structure type T,
    // is the member m defined? In Rhea syntax: `T .= m`
    struct concept_member_check : seq <
        identifier,
        pad <string <'.', '='>, ignored>,
        identifier
    > {};

    // A concept block is simply a comma separated list of
    // concept checks inside curly braces.
    struct concept_check : sor <
        concept_function_check,
        concept_member_check
    > {};

    struct concept_block : seq <
        one <'{'>,
        separator,
        list <concept_check, one <','>, ignored>,
        separator,
        one <'}'>
    > {};
}}

#endif /* RHEA_GRAMMAR_CONCEPTS_HPP */