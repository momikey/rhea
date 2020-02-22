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
        ModuleScopeNode();
        
        std::string name;
        std::unordered_map<std::string, ast::ASTNode*> symbol_table;

        ModuleScopeNode* parent;
        std::vector<std::unique_ptr<ModuleScopeNode>> children;
    };
}}

#endif /* RHEA_STATE_MODULE_NODE_HPP */