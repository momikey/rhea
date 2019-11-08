#ifndef RHEA_GRAMMAR_KEYWORDS_HPP
#define RHEA_GRAMMAR_KEYWORDS_HPP

#include <tao/pegtl.hpp>

namespace rhea { namespace grammar {
    // Language keywords
    struct kw_if : TAO_PEGTL_KEYWORD("if") {};
    struct kw_then : TAO_PEGTL_KEYWORD("then") {};
    struct kw_else : TAO_PEGTL_KEYWORD("else") {};

    struct kw_and : TAO_PEGTL_KEYWORD("and") {};
    struct kw_or : TAO_PEGTL_KEYWORD("or") {};
    struct kw_not : TAO_PEGTL_KEYWORD("not") {};

    struct kw_as : TAO_PEGTL_KEYWORD("as") {};

    struct kw_true : TAO_PEGTL_KEYWORD("true") {};
    struct kw_false : TAO_PEGTL_KEYWORD("false") {};
}}

#endif /* RHEA_GRAMMAR_KEYWORDS_HPP */