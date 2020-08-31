#ifndef RHEA_DEBUG_BUILD_AST_HPP
#define RHEA_DEBUG_BUILD_AST_HPP

#include <memory>
#include <iostream>

#include "../ast.hpp"

namespace rhea { namespace debug {
    std::unique_ptr<ast::ASTNode> build_ast(ast::parser_node* node)
    {
        return ast::build_ast(node);
    }

    std::ostream& dump_ast(std::ostream& os, ast::ASTNode* tree)
    {
        return os << tree->to_string();
    }
}}

#endif /* RHEA_DEBUG_BUILD_AST_HPP */