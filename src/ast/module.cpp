#include "ast/nodes/module.hpp"

#include <algorithm>
#include <iterator>

namespace rhea { namespace ast {
    Program::Program(child_vector<Statement>& ss)
    {
        // The usual "move a vector of unique_ptrs" dance
        std::move(ss.begin(), ss.end(), std::back_inserter(children));
    }

    std::string Program::to_string()
    {
        // Same logic as in FullyQualified (identifiers.cpp)
        std::string s;
        s.reserve(children.size()*32);    // profile this to find a nice default

        for (auto&& id : children)
        {
            s += ',';
            s += id->to_string();
        }

        return fmt::format("(Program{0})", s);
    }

    Import::Import(child_vector<Identifier>& ids, std::unique_ptr<ModuleName> m)
        : module(std::move(m))
    {
        // The usual.
        std::move(ids.begin(), ids.end(), std::back_inserter(imports));
    }

    std::string Import::to_string()
    {
        // The usual vector printing logic.
        std::string s;
        s.reserve(imports.size()*16);    // profile this to find a nice default

        for (auto&& id : imports)
        {
            s += ',';
            s += id->to_string();
        }

        return fmt::format("(Import,{0}{1})", module->to_string(), s);
    }

    Export::Export(child_vector<Identifier>& ids)
    {
        // The usual.
        std::move(ids.begin(), ids.end(), std::back_inserter(exports));
    }

    std::string Export::to_string()
    {
        // Same logic as in FullyQualified (identifiers.cpp)
        std::string s;
        s.reserve(exports.size()*16);    // profile this to find a nice default

        for (auto&& id : exports)
        {
            s += ',';
            s += id->to_string();
        }

        return fmt::format("(Export{0})", s);
    }
}}