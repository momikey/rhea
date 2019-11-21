#ifndef RHEA_GRAMMAR_STRINGS_HPP
#define RHEA_GRAMMAR_STRINGS_HPP

#include <tao/pegtl.hpp>

/*
 * String parsing is always hard and complex. For Rhea, we'll isolate
 * it in its own header. That's mostly to keep from cluttering up other
 * files, and for readability purposes.
 */

namespace rhea { namespace grammar {
    using namespace tao::pegtl;

    struct escape_character : one <
        '\'',
        '"',
        '\\',
        'a',
        'b',
        'f',
        'n',
        'r',
        't',
        'v',
        '0'
    > {};

    struct hex_byte : if_must <
        one <'x'>,
        xdigit,
        xdigit
    > {};

    struct unicode_4 : if_must <
        one <'u'>,
        rep <4, xdigit>
    > {};

    struct unicode_8 : if_must <
        one <'U'>,
        rep <8, xdigit>
    > {};

    // Any escaped sequence
    struct escaped : if_must <
        one <'\\'>,
        sor <
            hex_byte,
            unicode_4,
            unicode_8,
            escape_character
        >
    > {};

    // Non-line ending characters, becuase CR/LF can't be in a string.
    struct regular_character : not_one <'\r', '\n'> {};

    struct character : sor <escaped, regular_character> {};

    template <char Q>
    struct quoted_string : if_must <
        one <Q>,
        until < one <Q>, character >
    > {};

    // Rhea strings can be single or double quoted, with no real difference.
    struct string_literal : sor <
        quoted_string <'\''>,
        quoted_string <'"'>
    > {};
}}

#endif /* RHEA_GRAMMAR_STRINGS_HPP */