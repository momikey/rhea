#include "ast/nodes/structure_type.hpp"

#include <algorithm>
#include <iterator>

namespace rhea { namespace ast {
    Container::Container(child_vector<Expression>& es)
    {
        // The usual vector move operation.
        std::move(es.begin(), es.end(), std::back_inserter(items));
    }

    // Different node types are all serialized in much the same way,
    // except that we have to change the name of the type.
    std::string Container::to_string_base(std::string node_type)
    {
        std::string s;
        s.reserve(items.size()*20);    // profile this to find a nice default

        for (auto&& id : items)
        {
            s += ',';
            s += id->to_string();
        }

        return fmt::format("({0}{1})", node_type, s);
    }
}}