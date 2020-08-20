#include "ast/nodes/typenames.hpp"

#include <algorithm>
#include <iterator>

namespace rhea { namespace ast {

    // Definitions for generic types
    GenericTypename::GenericTypename(child_vector<Typename>& c)
    {
        std::move(c.begin(), c.end(), std::back_inserter(children));
    }

    std::string GenericTypename::to_string()
    {
        // Same logic as in FullyQualified (identifiers.cpp)
        std::string s;
        s.reserve(children.size()*16);    // profile this to find a nice default

        for (auto&& id : children)
        {
            s += ',';
            s += id->to_string();
        }

        return fmt::format("(GenericTypename{0})", s);
    }

    // Definitions for array types
    ArrayTypename::ArrayTypename(child_vector<Expression>& c)
    {
        std::move(c.begin(), c.end(), std::back_inserter(children));
    }

    std::string ArrayTypename::to_string()
    {
        // Same logic as elsewhere.
        std::string s;
        s.reserve(children.size()*16);    // profile this to find a nice default

        for (auto&& id : children)
        {
            s += ',';
            s += id->to_string();
        }

        return fmt::format("(ArrayTypename{0})", s);
    }

    // Definition for variants
    Variant::Variant(child_vector<Typename>& ts)
    {
        std::move(ts.begin(), ts.end(), std::back_inserter(children));
    }

    std::string Variant::to_string()
    {
        // Same logic as in FullyQualified (identifiers.cpp)
        std::string s;
        s.reserve(children.size()*16);    // profile this to find a nice default

        for (auto&& id : children)
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

    // Canonical name representations. These are intended for internal use, and
    // possibly for compiler output.
    std::string Typename::canonical_name() const
    {
        std::string result = name->canonical_name();

        if (generic_part != nullptr)
        {
            result += '<';

            for (auto&& ch : generic_part->children)
            {
                result += ch->canonical_name();
                result += ',';
            }

            auto& gb = result.back();
            gb = '>';
        }

        if (array_part != nullptr)
        {
            for (auto&& ch : array_part->children)
            {
                result += '[';

                // TODO: The array part is an arbitrary expression, but it should evaluate
                // to *something* integral. Working out exactly what is nontrivial, though.
                result += ch->to_string();

                result += ']';
            }
        }

        return result;
    }

    // Canonical representation for variants.
    std::string Variant::canonical_name() const
    {
        std::string result { "Variant(" };

        for (auto&& ch : children)
        {
            result += ch->canonical_name() + ',';
        }

        auto& back = result.back();
        back = ')';

        return result;
    }

    // Canonical representation for optionals.
    std::string Optional::canonical_name() const
    {
        return fmt::format("Optional({0})", type->canonical_name());
    }
}}