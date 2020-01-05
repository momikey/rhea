#ifndef RHEA_VISITOR_HPP
#define RHEA_VISITOR_HPP

#include "../compat.hpp"
#include "../ast.hpp"
#include "../visitor/visitor.hpp"

namespace rhea { namespace visitor {
    using util::any;
    using namespace ast;
    
    /*
     * The AST visitor, along with individual `visit` methods in each AST class,
     * allows us to implement this pattern using double dispatch, decoupling the
     * visitor logic from both the AST itself and the compiler passes that use it.
     * 
     * Basically, it works like this. The visitor has a `visit` method taking a
     * pointer to each different kind of AST node. These methods are virtual, and
     * any child class (a codegen visitor, for example) will override them to
     * implement custom logic. In the article where I learned this technique,
     * the author made the visitor subclasses friends of the implementation classes.
     * That's optional here, though. And it'll depend on what kind of state, etc.,
     * we need. Also, we return an `any` value instead of void (and we do the same
     * in the AST methods) to allow for a variety of situations. If this turns out
     * to be too slow, I may switch back to the more traditional visitor pattern.
     */
    struct Visitor
    {
        virtual any visit(ASTNode*) = 0;
        virtual any visit(Expression*) = 0;
        virtual any visit(Statement*) = 0;
        virtual any visit(Boolean*) = 0;
    };

    /*
     * Inner workings for a default visitor that does nothing for each node type.
     * Skip over this if you don't care about the gory details.
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
     */
    struct DefaultVisitor : internal::DefaultVisitor<
        ASTNode,
        Expression,
        Statement,
        Boolean
        /* All AST classes */
    >
    {};
}}

#endif /* RHEA_VISITOR_HPP */