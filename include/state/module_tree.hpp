#ifndef RHEA_STATE_MODULE_TREE_HPP
#define RHEA_STATE_MODULE_TREE_HPP

#include <string>
#include <memory>
#include <vector>

#include "../ast/nodes/node_base.hpp"

#include "module_node.hpp"

/*
 * The module scope tree contains symbol tables for each scope, nested in a tree
 * format for easy traversing.
 */
namespace rhea { namespace state {
    struct ModuleScopeTree
    {
        ModuleScopeTree(std::string n);
        ModuleScopeTree();

        // Create a new scope as a child of the current one.
        void begin_scope(std::string name);

        // End the current scope and return to its parent.
        void end_scope();

        // Find a symbol in the current scope or its ancestors. Returns a null pointer
        // if the symbol cannot be found.
        ast::ASTNode* find_symbol(std::string sym);

        // Add a symbol to the current scope. Throws an exception if trying to add
        // to a null pointer. (This can happen with unbalanced begin/end calls.)
        void add_symbol(std::string sym, ast::ASTNode* node);

        // The fully-qualified name of this module, or an empty string if it
        // represents a program instead.
        std::string name;

        // An owning pointer to the root node of the scope tree.
        std::unique_ptr<ModuleScopeNode> root;

        // Non-owning pointers to any modules that have been imported and processed.
        std::vector<ModuleScopeTree*> imports;

        // A container with the string names of all identifiers exported by this module.
        std::vector<std::string> exports;

        // State variable linking to the current scope.
        ModuleScopeNode* current_scope;
    };
}}

#endif /* RHEA_STATE_MODULE_TREE_HPP */