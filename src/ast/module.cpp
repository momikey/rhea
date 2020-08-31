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
        return fmt::format("(Program{0})", util::serialize_array(children, 32));
    }

    Module::Module(child_vector<Statement>& ss)
    {
        // The usual.
        std::move(ss.begin(), ss.end(), std::back_inserter(children));
    }

    std::string Module::to_string()
    {
        return fmt::format("(Module{0})", util::serialize_array(children, 32));
    }

    Import::Import(child_vector<Identifier>& ids, std::unique_ptr<ModuleName> m)
        : module(std::move(m))
    {
        // The usual.
        std::move(ids.begin(), ids.end(), std::back_inserter(imports));
    }

    std::string Import::to_string()
    {
        return fmt::format("(Import,{0}{1})",
            module->to_string(),
            util::serialize_array(imports, 32));
    }

    Export::Export(child_vector<Identifier>& ids)
    {
        // The usual.
        std::move(ids.begin(), ids.end(), std::back_inserter(exports));
    }

    std::string Export::to_string()
    {
        return fmt::format("(Export{0})", util::serialize_array(exports, 32));
    }
}}