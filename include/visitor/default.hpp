#ifndef RHEA_VISITOR_DEFAULT_HPP
#define RHEA_VISITOR_DEFAULT_HPP

#include "visitor.hpp"

namespace rhea { namespace visitor {
    /*
     * Inner workings for a default visitor that does nothing for each node type.
     * Skip over this if you don't care about the gory details.
     * 
     * Compiler passes can derive from `rhea::visitor::DefaultVisitor` to get a
     * do-nothing default for any type they don't want to handle. From there, they
     * can override the methods for the types they *do* care about, without worrying
     * about boilerplate or errors.
     */
    namespace internal {
        /*
         * To create a "default" visitor, we must override each method of the base
         * class. That's tedious and error-prone (and we're essentially doing the
         * same thing in the AST already), so I looked for a better way. This is what
         * I found.
         * 
         * This is another case where limiting Rhea to C++14 at the start hinders us,
         * because a variadic using declaration would make this far, far simpler.
         * Instead, we have to do a little dance with a variadic template.
         */

        // First, the declaration.
        template <typename... T>
        struct DefaultVisitor;

        // Then, the recursive specialization.
        template <typename T0, typename... T>
        struct DefaultVisitor<T0, T...> : DefaultVisitor<T...>
        {
            using DefaultVisitor<T...>::visit;
            any visit(T0*) override { return {}; }
        };

        // Last, we have the base case.
        template <typename T0>
        struct DefaultVisitor<T0> : Visitor
        {
            any visit(T0*) override { return {}; }
        };
    }

    /*
     * The default visitor does nothing for all node types. It can be used as
     * a base for compiler passes that don't care about certain types, such as
     * the declaration checker. (Or even codegen, as concepts, for instance,
     * produce no code themselves.)
     * 
     * The way it works is simple: derive from the internal implementation,
     * templated on every AST class that can be visited. This is still tedious,
     * but not as much as writing out (or copy-pasting) the same method 100 times.
     */
    struct DefaultVisitor : internal::DefaultVisitor<
        ASTNode,
        Expression,
        Statement,
        Boolean,
        Integer,
        Byte,
        Long,
        UnsignedInteger,
        UnsignedByte,
        UnsignedLong,
        Float,
        Double,
        String,
        Symbol,
        Nothing,
        Identifier,
        FullyQualified,
        RelativeIdentifier,

        BinaryOp,
        UnaryOp,
        TernaryOp,
        Member,
        Subscript,

        GenericTypename,
        Typename,
        Variant,
        Optional,
        Cast,
        TypeCheck,
        Alias,
        SymbolList,
        Enum,
        TypePair,

        BareExpression,
        If,
        While,
        For,
        With,
        Break,
        Continue,
        Match,
        On,
        When,
        TypeCase,
        Default,
        PredicateCall,
        NamedArgument,
        Call,
        Arguments,
        Condition,
        Def,
        GenericDef,
        Return,
        Extern,

        Block,
        Assign,
        CompoundAssign,
        TypeDeclaration,
        Variable,
        Constant,
        Do,
        
        Array,
        List,
        Tuple,
        DictionaryEntry,
        Dictionary,
        Structure,

        Try,
        Catch,
        Throw,
        Finally,

        Program,
        Module
        /* All AST classes */
    >
    {};
}}

#endif /* RHEA_VISITOR_DEFAULT_HPP */