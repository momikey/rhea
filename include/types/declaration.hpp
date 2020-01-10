#ifndef RHEA_STATE_DECLARATION_HPP
#define RHEA_STATE_DECLARATION_HPP

/*
 * Definitions needed to describe types as they are used by the compiler.
 */
namespace rhea { namespace types {

    // The type of declaration for a given symbol.
    enum class DeclarationType
    {
        Variable,
        Constant,
        Function,
        Generic,
        Structure,
        Enum,
        Alias,
        Concept
    };
}}

#endif /* RHEA_STATE_DECLARATION_HPP */