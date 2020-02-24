#ifndef RHEA_TYPES_MAPPER_HPP
#define RHEA_TYPES_MAPPER_HPP

#include <map>
#include <string>
#include <memory>

#include "types.hpp"
#include "../util/compat.hpp"

namespace rhea { namespace types {
    /*
    * The type mapper maps typename strings to objects that describe the types.
    */
    struct TypeMapper
    {
        TypeMapper();

        // Get the typeinfo object for a given typename string. If one hasn't been
        // declared, we return an unknown type rather than throwing an exception,
        // because this is technically a problem in the Rhea code, not the compiler.
        TypeInfo get_type_for(std::string s)
            { return is_type_defined(s) ? type_map[s] : TypeInfo(UnknownType()); }

        // Add a new type to the map of known types. This returns true if the
        // insertion succeeded.
        bool add_type_definition(std::string s, TypeInfo ti);

        // Remove the definition of the type with the given name.
        util::optional<TypeInfo> remove_type_definition(std::string s);

        // Does the given typename have a known mapping?
        // (We use the map::count method rather than find to avoid iterator stuff.)
        bool is_type_defined(std::string s) { return type_map.count(s) > 0; }

        std::map<std::string, TypeInfo> type_map;

        private:
        void insert_builtin_types();
    };
}}

#endif /* RHEA_TYPES_MAPPER_HPP */