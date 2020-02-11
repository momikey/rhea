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

    // Get the canonical name of a FullyQualified node
    std::string FullyQualified::canonical_name() const
    {
        std::string s;
        for (auto&& id : children)
        {
            s += id->name;
            s += ':';
        }

        // Remove trailing colon
        if (s.back() == ':')
        {
            s.pop_back();
        }

        return s;
    }

    // Getting the canonical name of a relative identifier requires us to know the
    // current module. If we don't for whatever reason, then we'll just let the
    // caller deal with it.
    std::string RelativeIdentifier::canonical_name() const
    {
        return fmt::format("{0}:{1}", module_name, identifier->canonical_name());
    }
}}