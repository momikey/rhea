#include "ast.hpp"
#include "types/types.hpp"

namespace rhea { namespace ast {
    types::TypeInfo UnaryOp::expression_type()
    {
        auto etype = operand->expression_type();
        auto etype_simple = util::get_if<types::SimpleType>(&etype);

        auto is_unsigned = (etype_simple != nullptr) &&
            (etype_simple->type == BasicType::UnsignedInteger ||
             etype_simple->type == BasicType::UnsignedByte ||
             etype_simple->type == BasicType::UnsignedLong);

        if (etype_simple != nullptr)
        {
            switch (op)
            {
                case UnaryOperators::Plus:
                    // Unary plus does nothing to simple numeric types
                    return etype;
                case UnaryOperators::Minus:
                    // Unary minus *does* convert an unsigned type to its signed counterpart
                    if (etype_simple->type == BasicType::UnsignedInteger)
                    {
                        return types::SimpleType(BasicType::Integer, true, true);
                    }
                    else if (etype_simple->type == BasicType::UnsignedLong)
                    {
                        return types::SimpleType(BasicType::Long, true, true);
                    }
                    else if (etype_simple->type == BasicType::UnsignedByte)
                    {
                        return types::SimpleType(BasicType::Byte, true, true);
                    }
                    else
                    {
                        return etype;
                    }
                case UnaryOperators::BooleanNot:
                    return types::SimpleType(BasicType::Boolean, false);
                case UnaryOperators::Coerce:
                    // Coercion operator always results in the "promoted" type.
                    // Operations that depend on it will inspect.
                    return types::SimpleType(BasicType::Promoted, false);
                // TODO: Other operators
                default:
                    throw unimplemented_type("Operation not implemented on type");
            }
        }
        else
        {
            // TODO: Handle other operand types
        }

        return types::UnknownType();
    }
}}