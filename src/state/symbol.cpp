#include "state/symbol.hpp"

namespace rhea { namespace state {
    void ScopeManager::add_symbol(SymbolEntry sym)
    {
        // If there are no scopes, we add a new unnamed one.
        if (m_stack.empty())
        {
            push();
        }

        m_stack.back().symbol_table[sym.name] = sym;
    }

    bool ScopeManager::is_local(std::string s)
    {
        const auto local = m_stack.back();
        return (local.symbol_table.find(s) != local.symbol_table.end());
    }

    SymbolSearchResult ScopeManager::find(std::string key)
    {
        SymbolSearchResult opt {};

        // We search backwards through the scope stack to find the latest declaration
        // of the desired symbol.
        for (auto s = m_stack.rbegin(); s != m_stack.rend(); ++s)
        {
            auto sym = s->symbol_table.find(key);

            if (sym != s->symbol_table.end())
            {
                opt = std::make_pair(std::ref(sym->second), s->name);
                break;
            }
        }

        return opt;
    }
}}