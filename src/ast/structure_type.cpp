#include "ast/nodes/structure_type.hpp"

#include <algorithm>
#include <iterator>

namespace rhea { namespace ast {
    Container::Container(child_vector<Expression>& es)
    {
        // The usual vector move operation.
        std::move(es.begin(), es.end(), std::back_inserter(items));
    }

    // Different node types are all serialized in much the same way,
    // except that we have to change the name of the type.
    std::string Container::to_string_base(std::string node_type)
    {
        return fmt::format("({0}{1})",
            node_type,
            util::serialize_array(items, 32));
    }

    // Definitions for structure definition
    Structure::Structure(std::unique_ptr<Identifier> n, child_vector<TypePair>& fs)
        : name(std::move(n))
    {
        std::move(fs.begin(), fs.end(), std::back_inserter(fields));
    }

    std::string Structure::to_string()
    {
        return fmt::format("(Structure,{0}{1})",
            name->to_string(),
            util::serialize_array(fields)); 
    }

    std::string DictionaryEntry::to_string()
    {
        auto ks = util::visit([&](auto const& v) { return v->to_string(); }, key);

        return fmt::format("(DictionaryEntry,{0},{1})", ks, value->to_string());
    }

    Dictionary::Dictionary(child_vector<DictionaryEntry>& es)
    {
        // The usual vector move operation, but we can't inherit it from Container
        // because of the differing item types.
        std::move(es.begin(), es.end(), std::back_inserter(items));
    }

    std::string Dictionary::to_string()
    {
        return fmt::format("(Dictionary{0})", util::serialize_array(items, 32));
    }
}}