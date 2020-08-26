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

        child_vector<Statement> children;
        
        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override;
    };

    class Module : public ASTNode
    {
        public:
        Module(child_vector<Statement>& ss);

        child_vector<Statement> children;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override;
    };

    // Helper for module names, which aren't exactly the same as either
    // simple or fully-qualified identifiers.
    class ModuleName : public ASTNode
    {
        public:
        ModuleName(std::unique_ptr<AnyIdentifier> id) : name(id->canonical_name()) {}

        const std::string name;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override
        { return fmt::format("(ModuleName,{0})", name); }
    };

    class ModuleDef : public Statement
    {
        public:
        ModuleDef(std::unique_ptr<ModuleName> n) : name(std::move(n)) {}

        const std::unique_ptr<ModuleName> name;
        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override
            { return fmt::format("(ModuleDef,{0})", name->to_string()); }
    };

    class Use : public Statement
    {
        public:
        Use(std::unique_ptr<ModuleName> m) : module(std::move(m)) {}

        const std::unique_ptr<ModuleName> module;
        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override
            { return fmt::format("(Use,{0})", module->to_string()); }
    };

    class Import : public Statement
    {
        public:
        Import(child_vector<Identifier>& ids, std::unique_ptr<ModuleName> m);

        child_vector<Identifier> imports;
        const std::unique_ptr<ModuleName> module;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override;
    };

    class Export : public Statement
    {
        public:
        Export(child_vector<Identifier>& ids);

        child_vector<Identifier> exports;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override;
    };
}}

#endif /* RHEA_NODES_MODULE_HPP */