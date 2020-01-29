#include "codegen/type_convert.hpp"

namespace rhea { namespace codegen {
    using llvm::Value;
    using types::TypeInfo;
    using types::SimpleType;

    Value* convert_type(CodeGenerator* gen, Value* value, TypeInfo from, TypeInfo to, bool explicit_ = false)
    {
        // Test to see if an implicit conversion is possible. If not, throw an error.
        if (!explicit_ && !internal::can_implicitly_convert(from, to))
        {
            throw syntax_error(fmt::format("No implicit conversion between types {0} and {1}",
                types::to_string(from), types::to_string(to)
            ));
        }

        return util::visit(
            [&] (auto& f, auto& t) { return internal::convert(gen, value, f, t); },
            from,
            to
        );
    }

    namespace internal {
        bool can_implicitly_convert(TypeInfo from, TypeInfo to)
        {
            auto from_simple = util::get_if<SimpleType>(&from);
            auto to_simple = util::get_if<SimpleType>(&to);

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

        template <>
        Value* convert(CodeGenerator* gen, Value* value, SimpleType from, SimpleType to)
        {
            using llvm::Type;

            // We'll get the trivial cased out of the way first.
            if (from.type == to.type)
            {
                return value;
            }

            // If we don't do anything, then we'll just return the original value,
            // but we'll most likely throw an exception if bad things happen.
            Value* ret = value;

            // We swtich on the *destination* type first, because that makes for clearer code.
            // Any inner switches are on the source type. Messy, but workable. Note that we already
            // accounted for the case of equivalent types above, so they're not included here.
            switch (to.type)
            {
                case BasicType::Integer:
                {
                    switch (from.type)
                    {
                        case BasicType::UnsignedInteger:
                            // No codegen needed
                            break;

                        case BasicType::Long:
                        case BasicType::UnsignedLong:
                            ret = gen->builder.CreateTrunc(value, Type::getInt32Ty(gen->context), "integercvt");
                            break;
                        
                        case BasicType::Byte:
                            ret = gen->builder.CreateSExt(value, Type::getInt32Ty(gen->context), "integercvt");
                            break;

                        case BasicType::UnsignedByte:
                        case BasicType::Boolean:
                            ret = gen->builder.CreateZExt(value, Type::getInt32Ty(gen->context), "integercvt");
                            break;

                        case BasicType::Float:
                            ret = gen->builder.CreateFPToSI(value, Type::getInt32Ty(gen->context), "integercvt");
                            break;

                        case BasicType::Double:
                            ret = gen->builder.CreateFPToSI(value, Type::getInt32Ty(gen->context), "integercvt");
                            break;
                            
                        default:
                            throw unimplemented_type("Unable to convert from type " + types::to_string(to));
                    }
                    break;
                }

                case BasicType::Byte:
                {
                    break;
                }

                case BasicType::Long:
                {
                    break;
                }

                case BasicType::UnsignedInteger:
                {
                    break;
                }

                case BasicType::UnsignedByte:
                {
                    break;
                }

                case BasicType::UnsignedLong:
                {
                    break;
                }
                
                case BasicType::Float:
                {
                    switch (from.type)
                    {
                        case BasicType::Byte:
                        case BasicType::Integer:
                        case BasicType::Long:
                            ret = gen->builder.CreateSIToFP(value, Type::getFloatTy(gen->context), "floatcvt");
                            break;

                        case BasicType::Boolean:
                        case BasicType::UnsignedByte:
                        case BasicType::UnsignedInteger:
                        case BasicType::UnsignedLong:
                            ret = gen->builder.CreateUIToFP(value, Type::getFloatTy(gen->context), "floatcvt");
                            break;

                        case BasicType::Double:
                            ret = gen->builder.CreateFPTrunc(value, Type::getFloatTy(gen->context), "floatcvt");
                            break;
                            
                        default:
                            throw unimplemented_type("Unable to convert from " + types::to_string(to));
                    }
                    break;
                }

                case BasicType::Double:
                {
                    switch (from.type)
                    {
                        case BasicType::Byte:
                        case BasicType::Integer:
                        case BasicType::Long:
                            ret = gen->builder.CreateSIToFP(value, Type::getDoubleTy(gen->context), "doublecvt");
                            break;

                        case BasicType::Boolean:
                        case BasicType::UnsignedByte:
                        case BasicType::UnsignedInteger:
                        case BasicType::UnsignedLong:
                            ret = gen->builder.CreateUIToFP(value, Type::getDoubleTy(gen->context), "doublecvt");
                            break;

                        case BasicType::Float:
                            ret = gen->builder.CreateFPExt(value, Type::getDoubleTy(gen->context), "doublecvt");
                            break;

                        default:
                            throw unimplemented_type("Unable to convert from " + types::to_string(to));
                    }
                    break;
                }

                case BasicType::Boolean:
                {
                    break;
                }

                default:
                    break;
            }

            // TODO: Strings, and maybe some kind of explicitly unsafe pointer conversion?

            return ret;
        }
    }
}}