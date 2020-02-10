#include "types/mapper.hpp"

namespace rhea { namespace types {
    TypeMapper::TypeMapper()
    {
        insert_builtin_types();
    }

    void TypeMapper::insert_builtin_types()
    {
        type_map["integer"] = SimpleType(BasicType::Integer, true, true);
        type_map["byte"] = SimpleType(BasicType::Byte, true, true);
        type_map["long"] = SimpleType(BasicType::Long, true, true);
        type_map["uinteger"] = SimpleType(BasicType::UnsignedInteger, true, true);
        type_map["ubyte"] = SimpleType(BasicType::UnsignedByte, true, true);
        type_map["ulong"] = SimpleType(BasicType::UnsignedLong, true, true);

        type_map["float"] = SimpleType(BasicType::Float, true, false);
        type_map["double"] = SimpleType(BasicType::Double, true, false);
        
        type_map["boolean"] = SimpleType(BasicType::Boolean, false, false);

        type_map["string"] = SimpleType(BasicType::String, true, true);
        type_map["symbol"] = SimpleType(BasicType::Symbol, true, true);
        type_map["any"] = SimpleType(BasicType::Any, true, true);

        type_map["ulong"] = NothingType();
    }

    bool TypeMapper::add_type_definition(std::string s, TypeInfo ti)
    {
        // This will not update an existing type definition, because we don't
        // want to risk allowing user code to, for instance, overwrite the
        // definition of "integer".
        if (!is_type_defined(s))
        {
            type_map[s] = ti;
            return true;
        }
        else
        {
            // This type is already defined. Issuing some sort of compiler
            // error is not really this class's responsibility.
            return false;
        }
    }

    util::optional<TypeInfo> TypeMapper::remove_type_definition(std::string s)
    {
        if (is_type_defined(s))
        {
            auto old_type = type_map[s];
            type_map.erase(s);
            return old_type;
        }
        else
        {
            // The type isn't defined, so this is a no-op that returns an
            // empty optional.
            return {};
        }
    }
}}