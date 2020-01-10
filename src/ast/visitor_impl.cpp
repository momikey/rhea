#include "ast.hpp"
#include "visitor/visitor.hpp"

/*
 * Each AST node class needs to have a visit method override, because
 * the way this double dispatch method works requires the child class's
 * `this`, even if we're calling through a pointer to base class. If we
 * were using C++17, this would be easy: we could just make a mixin with
 * a variadic using declaration. Alas, Rhea suuports C++14 at the moment,
 * which means we have to do things the hard way. Thus, this file is just
 * the various overrides for each class. They're all the same implementation.
 * 
 * Ref for the double dispatch method I'm using:
 * https://gieseanw.wordpress.com/2018/12/29/stop-reimplementing-the-virtual-table-and-start-using-double-dispatch/
 */

namespace rhea { namespace ast {
    using visitor::Visitor;
    using util::any;

    any ASTNode::visit(Visitor* v)
    { return v->visit(this); }

    any Expression::visit(Visitor* v)
    { return v->visit(this); }

    any Statement::visit(Visitor* v)
    { return v->visit(this); }

    any Boolean::visit(Visitor* v)
    { return v->visit(this); }

    template <>
    any Integer::visit(Visitor* v)
    { return v->visit(this); }

    template <>
    any Byte::visit(Visitor* v)
    { return v->visit(this); }

    template <>
    any Long::visit(Visitor* v)
    { return v->visit(this); }

    template <>
    any UnsignedInteger::visit(Visitor* v)
    { return v->visit(this); }

    template <>
    any UnsignedByte::visit(Visitor* v)
    { return v->visit(this); }

    template <>
    any UnsignedLong::visit(Visitor* v)
    { return v->visit(this); }

    template <>
    any Float::visit(Visitor* v)
    { return v->visit(this); }

    template <>
    any Double::visit(Visitor* v)
    { return v->visit(this); }

    any String::visit(Visitor* v)
    { return v->visit(this); }

    any Symbol::visit(Visitor* v)
    { return v->visit(this); }

    any Nothing::visit(Visitor* v)
    { return v->visit(this); }

    any BinaryOp::visit(Visitor* v)
    { return v->visit(this); }

    any TypeDeclaration::visit(Visitor* v)
    { return v->visit(this); }

    any Variable::visit(Visitor* v)
    { return v->visit(this); }

    any Constant::visit(Visitor* v)
    { return v->visit(this); }
}}