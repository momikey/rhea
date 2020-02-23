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
}}