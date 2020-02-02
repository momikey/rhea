#include "ast/nodes/exception.hpp"

#include <algorithm>
#include <iterator>

namespace rhea { namespace ast {
    Try::Try(statement_ptr b, child_vector<Catch>& cs, std::unique_ptr<Finally> f)
        : body(std::move(b)), finally_block(std::move(f))
    {
        // The usual "move a vector of unique_ptrs".
        std::move(cs.begin(), cs.end(), std::back_inserter(catches));
    }

    Try::Try(statement_ptr b, child_vector<Catch>& cs) : Try(std::move(b), cs, nullptr) {}

    std::string Try::to_string()
    {
        // Loop for the catch blocks, while other parts can just call their versions.
        std::string s;
        s.reserve(catches.size()*16);    // profile this to find a nice default

        for (auto&& id : catches)
        {
            s += ',';
            s += id->to_string();
        }

        return fmt::format("(Try,{0}{1},{2})", body->to_string(), s, 
            finally_block == nullptr ? "null" : finally_block->to_string());
    }
}}