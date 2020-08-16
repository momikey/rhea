#ifndef RHEA_NODES_MODULE_HPP
#define RHEA_NODES_MODULE_HPP

#include <memory>
#include <string>
#include <type_traits>
#include <fmt/format.h>

#include "node_base.hpp"
#include "identifiers.hpp"

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

        child_vector<Statement> children;
        
        util::any visit(visitor::Visitor* v) override;
    };

    // TODO module stuff
    class Module : public ASTNode
    {

        util::any visit(visitor::Visitor* v) override;
    };

    class Use : public Statement
    {
        public:
        Use(std::unique_ptr<AnyIdentifier> m) : module(std::move(m)) {}

        const std::unique_ptr<AnyIdentifier> module;
        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override
            { return fmt::format("(Use,{0})", module->to_string()); }
    };
}}

#endif /* RHEA_NODES_MODULE_HPP */