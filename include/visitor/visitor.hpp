#ifndef RHEA_VISITOR_HPP
#define RHEA_VISITOR_HPP

#include "../util/compat.hpp"
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
        virtual ~Visitor() {}

        virtual any visit(ASTNode*) = 0;
        virtual any visit(Expression*) = 0;
        virtual any visit(Statement*) = 0;
        virtual any visit(Boolean*) = 0;
        virtual any visit(Integer*) = 0;
        virtual any visit(Byte*) = 0;
        virtual any visit(Long*) = 0;
        virtual any visit(UnsignedInteger*) = 0;
        virtual any visit(UnsignedByte*) = 0;
        virtual any visit(UnsignedLong*) = 0;
        virtual any visit(Float*) = 0;
        virtual any visit(Double*) = 0;
        virtual any visit(String*) = 0;
        virtual any visit(Symbol*) = 0;
        virtual any visit(Nothing*) = 0;
        virtual any visit(Identifier*) = 0;
        virtual any visit(BinaryOp*) = 0;
        virtual any visit(UnaryOp*) = 0;
        virtual any visit(TernaryOp*) = 0;

        virtual any visit(BareExpression*) = 0;
        virtual any visit(TypeDeclaration*) = 0;
        virtual any visit(Variable* n) = 0;
        virtual any visit(Constant* n) = 0;
    };
}}

#endif /* RHEA_VISITOR_HPP */