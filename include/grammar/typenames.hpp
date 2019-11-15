#ifndef RHEA_GRAMMAR_TYPENAMES_HPP
#define RHEA_GRAMMAR_TYPENAMES_HPP

#include <tao/pegtl.hpp>

#include "tokens.hpp"

namespace rhea { namespace grammar {
    using namespace tao::pegtl;

    struct constant_expression;
    struct type_name;

    // Type specifiers
    struct generic_type : if_must <
        seq <
            one <'<'>,
            pad <
                list <type_name, one <','>, ignored>,
                ignored
            >
        >,
        one <'>'>
    > {};

    struct array_type : if_must <
        seq <
            one <'['>,
            pad <constant_expression, ignored>
        >,
        one <']'>
    > {};

    struct type_name : seq <
        fully_qualified,
        pad <opt <generic_type>, ignored>,
        pad <opt <array_type>, ignored>
    > {};

    // A type pair is a key-value pair where the value is a type name.
    // This is used in structure and concept definitions.
    struct type_pair : seq <
        identifier,
        separator,
        one <':'>,
        separator,
        type_name
    > {};
}}

#endif /* RHEA_GRAMMAR_TYPENAMES_HPP */