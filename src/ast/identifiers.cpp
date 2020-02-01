#include "ast/nodes/identifiers.hpp"

#include <algorithm>
#include <iterator>

namespace rhea { namespace ast {
    // Create a FullyQualified node from a vector of strings
    FullyQualified::FullyQualified(std::vector<std::string>& ns)
    {
        std::for_each(ns.begin(), ns.end(),
            [this] (const std::string& elem)
            {
                this->children.emplace_back(std::move(std::make_unique<Identifier>(elem)));
            }
        );
    }

    // Create a FullyQualified node from a vector of Identifier nodes
    FullyQualified::FullyQualified(child_vector<Identifier>& ns)
    {
        std::move(ns.begin(), ns.end(), std::back_inserter(children));
    }

    // Serialize a FullyQualified node
    std::string FullyQualified::to_string()
    {
        // The child identifiers always follow the node's name.
        // We can use this to simplify the logic.
        std::string s;
        s.reserve(children.size()*16);    // profile this to find a nice default

        for (auto&& id : children)
        {
            s += ',';
            s += id->name;
        }

        return fmt::format("(FullyQualified{0})", s);
    }
}}