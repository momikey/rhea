#ifndef RHEA_UTIL_SERIALIZE_ARRAY_HPP
#define RHEA_UTIL_SERIALIZE_ARRAY_HPP

#include <memory>
#include <string>
#include <vector>
#include <fmt/format.h>

#include "compat.hpp"

namespace rhea { namespace util {

    // Serialize the elements of an array in order, *preceded* by commas.
    // This is mainly intended for use in the AST printer, but other mdules
    // might find it helpful. The optional `reserve` parameter allows the caller
    // to reserve more or less space per element if optimizations are necessary.
    template <typename T>
    std::string serialize_array(std::vector<std::unique_ptr<T>>& children, size_t reserve = 16u)
    {
        std::string s;
        s.reserve(children.size() * reserve);

        // We precede each child node with a comma because the main use for
        // this function is to serialize AST nodes. In those, the array will
        // never be the first element printed.
        for (auto&& c : children)
        {
            s += ',';
            s += c->to_string();
        }

        return s;
    }

    template <typename... Ts>
    std::string serialize_array(std::vector<util::variant<Ts...>>& children, size_t reserve = 16u)
    {
        std::string s;
        s.reserve(children.size() * reserve);

        // Variants need slightly different logic for the visitor, but the
        // outcome is the same.        
        for (auto&& c : children)
        {
            s += ',';
            s += util::visit([](auto const& v) {
                return v->to_string(); }, c);
        }

        return s;
    }

}}
#endif /* RHEA_UTIL_SERIALIZE_ARRAY_HPP */