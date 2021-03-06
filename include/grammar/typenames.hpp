#ifndef RHEA_GRAMMAR_TYPENAMES_HPP
#define RHEA_GRAMMAR_TYPENAMES_HPP

#include <tao/pegtl.hpp>

#include "tokens.hpp"
#include "operator.hpp"
#include "expression_fwd.hpp"

namespace rhea { namespace grammar {
    using namespace tao::pegtl;

    struct type_name;

    // The "simple" builtin types for Rhea, i.e., those represented
    // by single identifiers.
    struct builtin_types : sor <
        kw_integer,
        kw_byte,
        kw_long,
        kw_uinteger,
        kw_ubyte,
        kw_ulong,
        kw_string,
        kw_any,
        kw_nothing        
    > {};

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
        return_type_operator,
        separator,
        type_name
    > {};

    struct ptr_reference_type : sor <kw_ref, kw_ptr> {};

    // struct simple_type_name : fully_qualified {};

    struct complex_type_lookahead : at <
        separator,
        sor <generic_type, array_type>
    > {};

    struct complex_type_name : seq <
        any_identifier,
        complex_type_lookahead,
        pad <opt <generic_type>, ignored>,
        pad <opt <
            list < array_type, separator >
        >, ignored>
    > {};

    struct pointer_or_reference_name : seq <
        ptr_reference_type,
        separator,
        type_name
    > {};

    struct simple_type_name : sor <
        builtin_types,
        any_identifier
    > {};

    struct either_type_name : sor <complex_type_name, simple_type_name> {};

    struct variant_type_list : list <type_name, one <','>, ignored> {};

    struct tagged_union : if_must <
        seq <
            one <'|'>,
            variant_type_list
        >,
        one <'|'>,
        not_at <separator, one <'?'> >
    > {};

    struct optional_type : if_must <
        seq <
            one <'|'>,
            at <until <one <'|'> >, separator, one <'?'> >,
            type_name
        >,
        one <'|'>,
        separator,
        one <'?'>
    > {};

    struct type_name : sor <
        pointer_or_reference_name,
        either_type_name,
        optional_type,
        tagged_union
    > {};

    // A type pair is a key-value pair where the value is a type name.
    // This is used in structure and concept definitions, as well as
    // catching exceptions.
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
        assignment_operator
    > {};

    // Type assertions are used in the match-type statement. We define
    // them as unable to use variants/optionals, since the whole purpose
    // of the construct is to let us match against those.
    struct type_assertion : seq <
        kw_type,
        separator,
        either_type_name
    > {};

    // Type and concept matching
    struct type_match : type_pair {};
}}

#endif /* RHEA_GRAMMAR_TYPENAMES_HPP */