#include "state/module_node.hpp"

namespace rhea { namespace state {
    ModuleScopeNode::ModuleScopeNode(std::string n, ModuleScopeNode* p)
        : name(n), parent(p)
    {}

    ModuleScopeNode::ModuleScopeNode(std::string n) : ModuleScopeNode(n, nullptr)
    {}

    ModuleScopeNode::ModuleScopeNode(ModuleScopeNode* p) : ModuleScopeNode("", p)
    {}

    ModuleScopeNode::ModuleScopeNode() : ModuleScopeNode("", nullptr)
    {}

    ast::ASTNode* ModuleScopeNode::find_symbol(std::string sym)
    {
        if (symbol_table.count(sym) > 0)
        {
            return symbol_table[sym];
        }
        else if (parent == nullptr)
        {
            return nullptr;
        }
        else
        {
            return parent->find_symbol(sym);
        }
    }
}}