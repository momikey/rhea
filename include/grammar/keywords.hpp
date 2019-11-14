#ifndef RHEA_GRAMMAR_KEYWORDS_HPP
#define RHEA_GRAMMAR_KEYWORDS_HPP

#include <tao/pegtl.hpp>

namespace rhea { namespace grammar {
    // Language keywords
    struct kw_if : TAO_PEGTL_KEYWORD("if") {};
    struct kw_then : TAO_PEGTL_KEYWORD("then") {};
    struct kw_else : TAO_PEGTL_KEYWORD("else") {};
    struct kw_unless : TAO_PEGTL_KEYWORD("unless") {};
    struct kw_while : TAO_PEGTL_KEYWORD("while") {};
    struct kw_for : TAO_PEGTL_KEYWORD("for") {};
    struct kw_in : TAO_PEGTL_KEYWORD("in") {};
    struct kw_do : TAO_PEGTL_KEYWORD("do") {};

    struct kw_and : TAO_PEGTL_KEYWORD("and") {};
    struct kw_or : TAO_PEGTL_KEYWORD("or") {};
    struct kw_not : TAO_PEGTL_KEYWORD("not") {};

    struct kw_as : TAO_PEGTL_KEYWORD("as") {};

    struct kw_true : TAO_PEGTL_KEYWORD("true") {};
    struct kw_false : TAO_PEGTL_KEYWORD("false") {};

    struct kw_const : TAO_PEGTL_KEYWORD("const") {};
    struct kw_var : TAO_PEGTL_KEYWORD("var") {};
}}

#endif /* RHEA_GRAMMAR_KEYWORDS_HPP */