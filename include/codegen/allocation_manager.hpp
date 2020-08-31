#ifndef RHEA_CODEGEN_ALLOCA_MGR_HPP
#define RHEA_CODEGEN_ALLOCA_MGR_HPP

#include <string>
#include <unordered_map>

#include <llvm/IR/Instructions.h>

#include "../util/compat.hpp"

/*
 * A manager to track allocations in LLVM IR. This is needed because variable
 * loads/stores must be able to "find" the `alloca` instruction that defined them.
 * 
 * Design-wise, it mostly mirrors the Rhea scope manager (state/symbol.hpp).
 */
namespace rhea { namespace codegen {
    using AllocaTable = std::unordered_map<std::string, llvm::AllocaInst*>;

    struct AllocationScope
    {
        std::string name;
        AllocaTable alloca_table;
    };

    struct AllocationManager
    {
        AllocationManager() : m_stack({ {"$global", {}} }) {}

        // Add a new allocation scope to the stack, optionally with a name.
        void push() { m_stack.push_back({}); }
        void push(std::string name) { m_stack.push_back({name, {}}); }

        // Delete the current scope. This is done at the end of a scoping
        // block to prevent contamination of unrelated blocks.
        void pop() { m_stack.pop_back(); }

        // Add a new entry to the current allocation scope.
        void add(std::string name, llvm::AllocaInst* ai);

        // Check to see if a variable has been allocated in the current scope.
        bool is_local(std::string s);

        // Find an entry in the current allocation table. If it doesn't exist there,
        // continue trying parent scopes until it is found.
        util::optional<llvm::AllocaInst*> find(std::string key);

        private:
        // We use a vector rather than a stack here, even though we call
        // it a stack. That's because the standard libaray's stack is
        // an adapter over vector (usually) which only allows access to
        // the topmost item. We need to be able to iterate through the
        // stack to find outer entries, so that won't work here.
        std::vector<AllocationScope> m_stack;
    };
}}

#endif /* RHEA_CODEGEN_ALLOCA_MGR_HPP */