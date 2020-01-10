#ifndef RHEA_STATE_SYMBOL_HPP
#define RHEA_STATE_SYMBOL_HPP

#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "../types/declaration.hpp"
#include "../types/types.hpp"
#include "../util/compat.hpp"

/*
 * The definition for an entry in the compiler's symbol tables.
 */
namespace rhea { namespace state {

    // Entries in the symbol table simply describe how to use
    // a particular declaration.
    struct SymbolEntry
    {
        std::string name;
        types::DeclarationType declaration;
        types::TypeInfo type_data;
        // more to add...
    };

    // A symbol table is just a hashtable of strings and symbol entries.
    // The keys are the "in-scope" names of the symbols, while the
    // entries themselves will hold "canonical" names.
    using SymbolTable = std::unordered_map<std::string, SymbolEntry>;

    // A scope holds a symbol table and any extra information needed
    // for type-checking and codegen.
    struct Scope
    {
        std::string name;
        SymbolTable symbol_table;
        // more to add...
    };

    // The symbol tables are created whenever a new scope is needed,
    // but inner scopes have to be able to access names from outer ones.
    // This class handles all that logic, as well as encapsulating all
    // the non-codegen aspects of symbols and scopes.
    struct ScopeManager
    {
        // Return the topmost (i.e., current) scope.
        Scope& current() { return m_stack.back(); }

        // Add a new scope to the stack. Note that this doesn't do any
        // copying of values; the searcher will look in parent scopes.
        void push() { m_stack.push_back({}); }
        void push(std::string name) { m_stack.push_back({name, {}}); }

        // Delete the current scope. This is done at the end of a scoping
        // block to prevent contamination of unrelated blocks.
        void pop() { m_stack.pop_back(); }

        // Add a new entry to the current scope.
        void add_symbol(SymbolEntry sym);

        // Find an entry in the symbol table. If it isn't in the most
        // local scope, then keep trying parent scopes.
        util::optional<std::reference_wrapper<SymbolEntry>> find(std::string key);

        private:
        // We use a vector rather than a stack here, even though we call
        // it a stack. That's because the standard libaray's stack is
        // an adapter over vector (usually) which only allows access to
        // the topmost item. We need to be able to iterate through the
        // stack to find outer entries, so that won't work here.
        std::vector<Scope> m_stack;
    };
}}

#endif /* RHEA_STATE_SYMBOL_HPP */