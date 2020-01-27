#include "codegen/type_convert.hpp"

namespace rhea { namespace codegen {
    using llvm::Value;

    Value* convert_type(Value* value, types::TypeInfo from, types::TypeInfo to, bool explicit_ = false)
    {
        // Test to see if an implicit conversion is possible. If not, throw an error.
        if (!explicit_ && !internal::can_implicitly_convert(from, to))
        {
            throw syntax_error(fmt::format("No implicit conversion between types {0} and {1}",
                types::to_string(from), types::to_string(to)
            ));
        }

        
    }

    namespace internal {
        bool can_implicitly_convert(types::TypeInfo from, types::TypeInfo to)
        {
            auto from_simple = util::get_if<types::SimpleType>(&from);
            auto to_simple = util::get_if<types::SimpleType>(&to);

            // Implicit conversion is a no-go by default.
            auto cvt = false;

            // Most implicit conversions are between simple types.
            if (from_simple != nullptr && to_simple != nullptr)
            {
                // We can always convert from a type to itself, so get that out of the way.
                if (from_simple->type == to_simple->type)
                {
                    cvt = true;
                }

                // Otherwise, the only implicit conversions allowed are:
                // (u)byte -> (u)integer or (u)long;
                // float -> double
                // (unsigned <-> signed is difficult because of overflow)
                switch (from_simple->type)
                {
                    case BasicType::Byte:
                        cvt = (to_simple->type == BasicType::Integer || to_simple->type == BasicType::Long) ;
                        break;
                    case BasicType::Integer:
                        cvt = (to_simple->type == BasicType::Long);
                        break;
                    case BasicType::Float:
                        cvt = (to_simple->type == BasicType::Double);
                        break;
                    default:
                        break;
                }
            }

            // TODO: Handle references. These should be transparent to user code.

            return cvt;
        }
    }
}}