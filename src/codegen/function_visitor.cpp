#include "codegen/function_visitor.hpp"
#include "codegen/generator.hpp"

namespace rhea { namespace codegen {
    using namespace rhea::ast;

    // Most of these just call into the child nodes for now, searching for returns.

    any FunctionVisitor::visit(If* n)
    {
        n->then_case->visit(this);
        n->else_case->visit(this);

        return {};
    }

    any FunctionVisitor::visit(While* n)
    {
        n->body->visit(this);

        return {};
    }

    any FunctionVisitor::visit(For* n)
    {
        n->body->visit(this);

        return {};
    }

    any FunctionVisitor::visit(With* n)
    {
        n->body->visit(this);

        return {};
    }

    any FunctionVisitor::visit(Match* n)
    {
        std::for_each(n->cases.begin(), n->cases.end(),
            [&] (auto& c) { c->visit(this); }
        );

        return {};
    }

    any FunctionVisitor::visit(On* n)
    {
        n->body->visit(this);

        return {};
    }

    any FunctionVisitor::visit(When* n)
    {
        n->body->visit(this);

        return {};
    }

    any FunctionVisitor::visit(TypeCase* n)
    {
        n->body->visit(this);

        return {};
    }

    any FunctionVisitor::visit(Default* n)
    {
        n->body->visit(this);

        return {};
    }

    any FunctionVisitor::visit(Def* n)
    {
        n->body->visit(this);

        return {};
    }

    any FunctionVisitor::visit(GenericDef* n)
    {
        n->body->visit(this);

        return {};
    }

    any FunctionVisitor::visit(Return* n)
    {
        potential_return_types.push_back(n->value->expression_type());

        return {};
    }

    any FunctionVisitor::visit(Block* n)
    {
        std::for_each(n->children.begin(), n->children.end(),
            [&] (auto& c) { c->visit(this); }
        );

        return {};
    }

    any FunctionVisitor::visit(Try* n)
    {
        n->body->visit(this);

        return {};
    }

    any FunctionVisitor::visit(Catch* n)
    {
        n->body->visit(this);

        return {};
    }

    any FunctionVisitor::visit(Finally* n)
    {
        n->body->visit(this);

        return {};
    }

}}