#ifndef RHEA_STATE_SYMBOL_HPP
#define RHEA_STATE_SYMBOL_HPP

#include <string>
#include <unordered_map>
#include <vector>

// Optionals aren't until C++17, but the Boost one works about the same.
#ifdef __cpp_lib_optional
#include <optional>
using std::optional;
#else
#include <boost/optional/optional.hpp>
using boost::optional;
#endif

#include "types.hpp"

/*
 * The definition for an entry in the compiler's symbol tables.
 */
namespace rhea { namespace state {

    // Entries in the symbol table simply describe how to use
    // a particular declaration.
    struct SymbolEntry
    {
        std::string name;
        DeclarationType type;
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
        SymbolTable symbol_table;
        // more to add...
    };

    // The symbol tables are created whenever a new scope is needed,
    // but inner scopes have to be able to access names from outer ones.
    // This class handles all that logic, as well as encapsulating all
    // the non-codegen aspects of symbols and scopes.
    struct ScopeStack
    {
        // Return the topmost (i.e., current) scope.
        Scope& current() { return m_stack.back(); }

        // Add a new scope to the stack. Note that this doesn't do any
        // copying of values; the searcher will look in parent scopes.
        void push() { m_stack.push_back({}); }

        // Delete the current scope. This is done at the end of a scoping
        // block to prevent contamination of unrelated blocks.
        void pop() { m_stack.pop_back(); }

        // Find an entry in the symbol table. If it isn't in the most
        // local scope, then keep trying parent scopes.
        optional<SymbolEntry&> find(std::string key);

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