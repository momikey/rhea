#ifndef RHEA_GRAMMAR_KEYWORDS_HPP
#define RHEA_GRAMMAR_KEYWORDS_HPP

#include <tao/pegtl.hpp>

namespace rhea { namespace grammar {
    ////
    // Language keywords
    ////

    // Control flow
    struct kw_if : TAO_PEGTL_KEYWORD("if") {};
    struct kw_then : TAO_PEGTL_KEYWORD("then") {};
    struct kw_else : TAO_PEGTL_KEYWORD("else") {};
    struct kw_unless : TAO_PEGTL_KEYWORD("unless") {};
    struct kw_while : TAO_PEGTL_KEYWORD("while") {};
    struct kw_for : TAO_PEGTL_KEYWORD("for") {};
    struct kw_in : TAO_PEGTL_KEYWORD("in") {};
    struct kw_do : TAO_PEGTL_KEYWORD("do") {};
    struct kw_with : TAO_PEGTL_KEYWORD("with") {};
    struct kw_match: TAO_PEGTL_KEYWORD("match") {};
    struct kw_on : TAO_PEGTL_KEYWORD("on") {};
    struct kw_when : TAO_PEGTL_KEYWORD("when") {};
    struct kw_default : TAO_PEGTL_KEYWORD("default") {};
    struct kw_break : TAO_PEGTL_KEYWORD("break") {};
    struct kw_continue : TAO_PEGTL_KEYWORD("continue") {};

    // Boolean operations
    struct kw_and : TAO_PEGTL_KEYWORD("and") {};
    struct kw_or : TAO_PEGTL_KEYWORD("or") {};
    struct kw_not : TAO_PEGTL_KEYWORD("not") {};
    struct kw_true : TAO_PEGTL_KEYWORD("true") {};
    struct kw_false : TAO_PEGTL_KEYWORD("false") {};

    // Types
    struct kw_type : TAO_PEGTL_KEYWORD("type") {};
    struct kw_as : TAO_PEGTL_KEYWORD("as") {};
    struct kw_is : TAO_PEGTL_KEYWORD("is") {};

    // Variable declaration
    struct kw_const : TAO_PEGTL_KEYWORD("const") {};
    struct kw_var : TAO_PEGTL_KEYWORD("var") {};

}}

#endif /* RHEA_GRAMMAR_KEYWORDS_HPP */