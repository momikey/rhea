#ifndef RHEA_STATE_MODULE_TREE_HPP
#define RHEA_STATE_MODULE_TREE_HPP

#include <string>
#include <memory>
#include <vector>

#include "module_node.hpp"

/*
 * The module scope tree contains symbol tables for each scope, nested in a tree
 * format for easy traversing.
 */
namespace rhea { namespace state {
    struct ModuleScopeTree
    {
        ModuleScopeTree();

        std::string name;
        std::unique_ptr<ModuleScopeNode> root;
        std::vector<ModuleScopeTree*> imports;
        std::vector<std::string> exports;

        ModuleScopeNode* current_scope;

        // Create a new scope as a child of the current one
        void begin_scope(std::string name = "(unnamed)");

        // End the current scope and return to its parent
        void end_scope();
    };
}}

#endif /* RHEA_STATE_MODULE_TREE_HPP */