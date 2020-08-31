#ifndef RHEA_STATE_MODULE_NODE_HPP
#define RHEA_STATE_MODULE_NODE_HPP

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

#include "../ast/nodes/node_base.hpp"

/*
 * Nodes for the module symbol table/scope tree.
 */
namespace rhea { namespace state {
    struct ModuleScopeNode
    {
        ModuleScopeNode(std::string n, ModuleScopeNode* p);
        
        ModuleScopeNode();
        ModuleScopeNode(ModuleScopeNode* p);
        ModuleScopeNode(std::string n);

        // Find an AST node in this scope or its ancestors. Some parts of the compiler
        // may need to do this with only a pointer to a scope, so we add a method here.
        ast::ASTNode* find_symbol(std::string sym);
        
        // The name of this scope. Most scopes will be unnamed, but something such as
        // a function definition will have its name here.
        std::string name;

        // The symbol table for this scope, linking identifier names defined in the
        // scope with the AST nodes that define them.
        std::unordered_map<std::string, ast::ASTNode*> symbol_table;

        // Non-owning pointer to this scope node's parent.
        ModuleScopeNode* parent;

        // Owning pointers to this scope node's children.
        std::vector<std::unique_ptr<ModuleScopeNode>> children;
    };
}}

#endif /* RHEA_STATE_MODULE_NODE_HPP */