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
        // In this and the similar method for relative identifiers below,
        // we can't use the generic `util::serialize_array()` function.
        // That's because we aren't serializing the child *nodes*, merely
        // their *names*.
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

    RelativeIdentifier::RelativeIdentifier(std::unique_ptr<Identifier> id, std::string mn)
        : module_name(mn)
    {
        children.emplace_back(std::move(id));
    }

    RelativeIdentifier::RelativeIdentifier(std::unique_ptr<FullyQualified> id, std::string mn)
        : module_name(mn)
    {
        // We need a slightly different setup here, because we're taking
        // the children of the fully-qualified identifier.
        std::move(id->children.begin(), id->children.end(), std::back_inserter(children));
    }

    // Serialize a RelativeIdentifier node
    std::string RelativeIdentifier::to_string()
    {
        std::string s;
        s.reserve(children.size()*16);    // profile this to find a nice default

        for (auto&& id : children)
        {
            s += ',';
            s += id->name;
        }

        return fmt::format("(RelativeIdentifier{0})", s);
    }

    // Getting the canonical name of a relative identifier requires us to know the
    // current module. If we don't for whatever reason, then we'll just let the
    // caller deal with it.
    std::string RelativeIdentifier::canonical_name() const
    {
        // Do we need a helper for the colon-separated version? It's only used in
        // identifiers, so maybe not.
        std::string s;
        s.reserve(children.size()*16);

        for (auto&& id : children)
        {
            s += ':';
            s += id->name;
        }

        return fmt::format("{0}{1}", module_name, s);
    }
}}