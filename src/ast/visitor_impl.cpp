#include "ast.hpp"
#include "visitor/visitor.hpp"

namespace rhea { namespace ast {
    void ASTNode::visit(visitor::Visitor* v)
    { v->visit(this); }
}}