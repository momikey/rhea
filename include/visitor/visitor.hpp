#ifndef RHEA_VISITOR_HPP
#define RHEA_VISITOR_HPP

#include "../compat.hpp"
#include "../ast.hpp"
#include "../visitor/visitor.hpp"

namespace rhea { namespace visitor {
    using util::any;
    
    struct Visitor
    {
        virtual any visit(ast::ASTNode*) = 0;
        virtual any visit(ast::Expression*) = 0;
        virtual any visit(ast::Statement*) = 0;
        virtual any visit(ast::Boolean*) = 0;
    };
}}

#endif /* RHEA_VISITOR_HPP */