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
        return fmt::format("(Block{0})", util::serialize_array(children));
    }
}}