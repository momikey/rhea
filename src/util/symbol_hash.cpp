#include "util/symbol_hash.hpp"

namespace rhea { namespace util {
    uint64_t symbol_hash(std::string str)
    {
        uint64_t result = FNV_OFFSET_BASIS;

        for (auto i = 0u; i < str.size(); ++i)
        {
            result = (result ^ str[i]) * FNV_PRIME;
        }

        return result;
    }
}}
