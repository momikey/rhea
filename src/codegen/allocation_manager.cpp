#include "codegen/allocation_manager.hpp"

namespace rhea { namespace codegen {
    using llvm::AllocaInst;

    void AllocationManager::add(std::string name, AllocaInst* ai)
    {
        if (m_stack.empty())
        {
            push();
        }

        m_stack.front().alloca_table[name] = ai;
    }

    bool AllocationManager::is_local(std::string s)
    {
        const auto local = m_stack.back();
        return (local.alloca_table.find(s) != local.alloca_table.end());
    }

    util::optional<llvm::AllocaInst*> AllocationManager::find(std::string key)
    {
        util::optional<llvm::AllocaInst*> opt({});

        // We search backwards through the scope stack to find the latest declaration
        // of the desired symbol.
        for (auto s = m_stack.rbegin(); s != m_stack.rend(); ++s)
        {
            auto ai = s->alloca_table.find(key);

            if (ai != s->alloca_table.end())
            {
                opt = ai->second;
                break;
            }
        }

        return opt;
    }
}}