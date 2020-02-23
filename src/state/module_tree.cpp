#include "state/module_tree.hpp"

namespace rhea { namespace state {
    ModuleScopeTree::ModuleScopeTree(std::string n)
        : name(n)
    {
        root = std::make_unique<ModuleScopeNode>("global", nullptr);
        current_scope = root.get();
    }

    ModuleScopeTree::ModuleScopeTree() : ModuleScopeTree("")
    {}

    void ModuleScopeTree::begin_scope(std::string name = "(unnamed)")
    {
        auto new_scope = std::make_unique<ModuleScopeNode>(name, current_scope);

        // Get a non-owning pointer to store as the new current scope.
        auto ptr = new_scope.get();

        current_scope->children.push_back(std::move(new_scope));
        current_scope = ptr;
    }

    void ModuleScopeTree::end_scope()
    {
        current_scope = current_scope->parent;
    }

    void ModuleScopeTree::add_symbol(std::string sym, ast::ASTNode* node)
    {
        if (current_scope->symbol_table.count(sym) > 0)
        {
            // This symbol already exists in this scope, which possibly indicates
            // a redefinition. That's okay with functions, because they can be
            // overloaded. Variables and constants, on the other hand, can't,
            // so we need to detect that.

            // TODO
        }
        else
        {
            current_scope->symbol_table[sym] = node;
        }
    }

    ast::ASTNode* ModuleScopeTree::find_symbol(std::string sym)
    {
        auto cs = current_scope;
        ast::ASTNode* result = nullptr;

        while (result == nullptr && cs != nullptr)
        {
            if (cs->symbol_table.count(sym) > 0)
            {
                result = cs->symbol_table[sym];
            }
            else
            {
                cs = cs->parent;
            }
        }

        return result;
    }
}}