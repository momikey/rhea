#include "ast/nodes/simple_statement.hpp"

#include <algorithm>
#include <iterator>

namespace rhea { namespace ast {
    Block::Block(child_vector<Statement>& ss)
    {
        // The usual "move a vector of unique_ptrs" dance
        std::move(ss.begin(), ss.end(), std::back_inserter(children));
    }

    std::string Block::to_string()
    {
        // Same logic as in FullyQualified (identifiers.cpp)
        std::string s;
        s.reserve(children.size()*16);    // profile this to find a nice default

        for (auto&& id : children)
        {
            s += ',';
            s += id->to_string();
        }

        return fmt::format("(Block{0})", s);
    }
}}