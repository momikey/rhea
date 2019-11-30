#include "ast/nodes/identifiers.hpp"

#include <algorithm>

namespace rhea { namespace ast {
    FullyQualified::FullyQualified(std::vector<std::string>& ns)
    {
        std::for_each(ns.begin(), ns.end(),
            [this] (const std::string& elem)
            {
                this->m_children.emplace_back(std::move(std::make_unique<Identifier>(elem)));
            }
        );
    }
}}