#include "ast/nodes/typenames.hpp"

#include <algorithm>
#include <iterator>

namespace rhea { namespace ast {
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
}}