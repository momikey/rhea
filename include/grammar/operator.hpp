#ifndef RHEA_GRAMMAR_OPERATOR_HPP
#define RHEA_GRAMMAR_OPERATOR_HPP

#include <tao/pegtl.hpp>

namespace rhea { namespace grammar {
    using namespace tao::pegtl;

    ////
    // Rules to match Rhea operators. We define these to make
    // AST generation a little easier, and so that the grammar
    // is a bit more expressive.
    ////

    // Note that we sometimes define the same operator with two
    // or more different names. This is intentional: they're used
    // at different points in the grammar.

    struct dot_operator : one <'.'> {};

    struct coerce_operator : one <'^'> {};

    struct bitnot_operator : one <'~'> {};

    struct dereference_operator : one <'*'> {};

    struct unary_plus_operator : one <'+'> {};

    struct unary_minus_operator : one <'-'> {};

    struct exponent_operator : TAO_PEGTL_STRING("**") {};

    // We use lookahead to distinguish multiplication from exponentiation
    struct multiply_operator : seq< one <'*'>, not_at <one <'*'>>> {};

    struct divide_operator : one <'/'> {};

    struct modulus_operator : one <'%'> {};

    struct add_operator : one <'+'> {};

    struct subtract_operator : one <'-'> {};

    struct left_shift_operator : TAO_PEGTL_STRING("<<") {};

    struct right_shift_operator : TAO_PEGTL_STRING(">>") {};
    
    struct equals_operator : TAO_PEGTL_STRING("==") {};

    struct not_equal_operator : TAO_PEGTL_STRING("!=") {};

    struct greater_equal_operator : TAO_PEGTL_STRING(">=") {};

    struct less_equal_operator : TAO_PEGTL_STRING("<=") {};

    // We have to use lookahead here so that these won't
    // get picked up ahead of the shift operators below.
    struct less_than_operator : seq < one <'<'>, not_at < one <'<'> > > {};

    struct greater_than_operator : seq < one <'>'>, not_at < one <'>'> > > {};

    struct bitand_operator : one <'&'> {};

    struct bitor_operator : one <'|'> {};

    struct bitxor_operator : one <'^'> {};

    struct return_type_operator : string <'-', '>'> {};

    struct assignment_operator : one <'='> {};

    struct concept_match_operator : string <'~', '>'> {};

    struct function_match_operator : string <'=', '>'> {};

    struct member_match_operator : string <'.', '='> {};
}}

#endif /* RHEA_GRAMMAR_OPERATOR_HPP */