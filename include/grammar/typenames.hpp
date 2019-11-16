#ifndef RHEA_GRAMMAR_TYPENAMES_HPP
#define RHEA_GRAMMAR_TYPENAMES_HPP

#include <tao/pegtl.hpp>

#include "tokens.hpp"
#include "expression_fwd.hpp"

namespace rhea { namespace grammar {
    using namespace tao::pegtl;

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

    struct return_type_spec : seq <
        string <'-', '>'>,
        separator,
        type_name
    > {};

    struct ptr_reference_type : sor <kw_ref, kw_ptr> {};

    struct simple_type_name : fully_qualified {};

    struct complex_type_name : seq <
        fully_qualified,
        at <
            separator,
            sor <generic_type, /*return_type_spec,*/ array_type>
        >,
        pad <opt <generic_type>, ignored>,
        // pad <opt <return_type_spec>, ignored>,
        pad <opt <array_type>, ignored>
    > {};

    struct either_type_name : sor <complex_type_name, simple_type_name> {};

    struct type_name : sor <
        seq <
            ptr_reference_type,
            separator,
            type_name
        >,
        either_type_name
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

    // A constructor expression calls the `new$` operator, creating
    // an instance of a type. As far sa the parser is concerned, it's
    // just a typename called as a function. However, a "simple" type
    // is just an identifier, which we already handle.
    struct constructor_expression : seq <
        complex_type_name,
        separator,
        function_call_expr
    > {};

    // Type definitions come in two forms: structures and enums. Enums
    // are simpler, in that they're just a symbol list. Structures take
    // a list of type pairs. Both start the same way.
    struct type_definition_lhs : seq <
        kw_type,
        separator,
        identifier,
        separator,
        one <'='>
    > {};
}}

#endif /* RHEA_GRAMMAR_TYPENAMES_HPP */