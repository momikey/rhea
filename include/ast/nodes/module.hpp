#ifndef RHEA_NODES_MODULE_HPP
#define RHEA_NODES_MODULE_HPP

#include <memory>
#include <string>
#include <type_traits>
#include <fmt/format.h>

#include "node_base.hpp"

/*
 * AST nodes for top-level structures (modules and programs), as well as the
 * various module-related constructs, such as import and export statements.
 */
namespace rhea { namespace ast {
    class Program : public ASTNode
    {
        public:
        Program(child_vector<Statement>& ss);

        std::string to_string() override;

        private:
        child_vector<Statement> children;
    };

    // TODO module stuff
}}

#endif /* RHEA_NODES_MODULE_HPP */