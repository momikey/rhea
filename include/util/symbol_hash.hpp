#ifndef RHEA_UTIL_SYMBOL_HASH_HPP
#define RHEA_UTIL_SYMBOL_HASH_HPP

#include <string>

/*
 * Although C++ defines a standard hashing function (rather, a functor),
 * it is implementation-defined. For Rhea symbols, we need something that
 * is more fixed. That is, a symbol should always hash to the same value.
 * (The only useful operation on symbols is equality checking, so this is
 * very important.)
 * 
 * Here, we define a simple FNV hash for symbols that operates on their
 * names as strings. Nothing too out of the ordinary, and it might be better
 * to get a library instead.
 */
namespace rhea { namespace util {
    // Magic constants for the hash function (we use 64-bit here)
    constexpr uint64_t FNV_OFFSET_BASIS = 0xcbf29ce484222325;
    constexpr uint64_t FNV_PRIME = 0x00000100000001b3;

    uint64_t symbol_hash(std::string str);
}}

#endif /* RHEA_UTIL_SYMBOL_HASH_HPP */