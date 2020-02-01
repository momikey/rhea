#include "ast/nodes/control_statement.hpp"

#include <algorithm>
#include <iterator>

namespace rhea { namespace ast {
    With::With(child_vector<TypePair>& ds, statement_ptr b)
        : body(std::move(b))
    {
        // This is the same logic as in a few other nodes,
        // but with the added wrinkle of the body init above.
        std::move(ds.begin(), ds.end(), std::back_inserter(declarations));
    }

    std::string With::to_string()
    {
        // Mostly the same as in e.g., FullyQualified, but we also
        // have to account for the body in the final string.
        std::string s;
        s.reserve(declarations.size()*16);    // profile this to find a nice default

        for (auto&& id : declarations)
        {
            s += ',';
            s += id->to_string();
        }

        return fmt::format("(With{0},{1})", s, body->to_string());
    }
}}