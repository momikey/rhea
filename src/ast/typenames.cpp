#include "ast/nodes/typenames.hpp"

#include <algorithm>
#include <iterator>

namespace rhea { namespace ast {

    // Definitions for generic types
    GenericTypename::GenericTypename(child_vector<Typename>& c)
    {
        std::move(c.begin(), c.end(), std::back_inserter(m_children));
    }

    std::string GenericTypename::to_string()
    {
        // Same logic as in FyllyQualified (identifiers.cpp)
        std::string s;
        s.reserve(m_children.size()*16);    // profile this to find a nice default

        for (auto&& id : m_children)
        {
            s += ',';
            s += id->to_string();
        }

        return fmt::format("(GenericTypename{0})", s);
    }

    // Definition for variants
    Variant::Variant(child_vector<Typename>& ts)
    {
        std::move(ts.begin(), ts.end(), std::back_inserter(m_children));
    }

    std::string Variant::to_string()
    {
        // Same logic as in FyllyQualified (identifiers.cpp)
        std::string s;
        s.reserve(m_children.size()*16);    // profile this to find a nice default

        for (auto&& id : m_children)
        {
            s += ',';
            s += id->to_string();
        }

        return fmt::format("(Variant{0})", s);        
    }

    // Definition for symbol lists
    SymbolList::SymbolList(std::vector<std::string>& ss)
    {
        for (auto&& s : ss)
        {
            auto p = std::make_unique<Symbol>(s);
            symbols.emplace_back(std::move(p));
        }
    }

    std::string SymbolList::to_string()
    {
        // Same logic as other "vector to string" nodes
        std::string s;

        // Profile this to find a nice default, but it'll be lower than others,
        // because enum symbol names are generally shorter
        s.reserve(symbols.size()*8);

        for (auto&& id : symbols)
        {
            s += ',';
            s += id->to_string();
        }

        return fmt::format("(SymbolList{0})", s); 
    }
}}