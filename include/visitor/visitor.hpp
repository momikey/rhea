#ifndef RHEA_VISITOR_HPP
#define RHEA_VISITOR_HPP

#include "../ast.hpp"

namespace rhea { namespace visitor {
    struct Visitor
    {
        virtual void visit(ast::ASTNode*) = 0;
    };
}}

#endif /* RHEA_VISITOR_HPP */