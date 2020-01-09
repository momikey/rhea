#include "codegen/code_visitor.hpp"
#include "codegen/generator.hpp"

namespace rhea { namespace codegen {
    using namespace rhea::ast;
    using llvm::Value;

    // Helper functions to reduce repetition. If we break the methods out into
    // their own files in the future (which may be necessary), we'll need to
    // move these into their own source/header pair.
    namespace internal {
        template <typename Int, int Width, bool Signed>
        Value* integral_value(Int val, CodeGenerator* gen)
        {
            return llvm::ConstantInt::get(
                gen->context,
                llvm::APInt(Width, val, Signed)
            );
        }

        template <typename Float>
        Value* fp_value(Float val, CodeGenerator* gen)
        {
            return llvm::ConstantFP::get(
                gen->context,
                llvm::APFloat(val)
            );
        }
    }

    any CodeVisitor::visit(Boolean* n)
    {
        Value* ret;
        if (n->value)
        {
            ret = llvm::ConstantInt::getTrue(generator->context);
        }
        else
        {
            ret = llvm::ConstantInt::getFalse(generator->context);
        }

        return ret;
    }

    any CodeVisitor::visit(Integer* n)
    {
        auto ret = internal::integral_value<
            Integer::template_type,
            32,     // TODO: Magic number
            true
        >(n->value, generator);

        return ret;
    }

    any CodeVisitor::visit(Byte* n)
    {
        auto ret = internal::integral_value<
            Byte::template_type,
            8,      // TODO: Magic number
            true
        >(n->value, generator);

        return ret;
    }

    any CodeVisitor::visit(Long* n)
    {
        auto ret = internal::integral_value<
            Long::template_type,
            64,     // TODO: Magic number
            true
        >(n->value, generator);

        return ret;
    }

    any CodeVisitor::visit(UnsignedInteger* n)
    {
        auto ret = internal::integral_value<
            UnsignedInteger::template_type,
            32,     // TODO: Magic number
            false
        >(n->value, generator);

        return ret;
    }

    any CodeVisitor::visit(UnsignedByte* n)
    {
        auto ret = internal::integral_value<
            UnsignedByte::template_type,
            8,      // TODO: Magic number
            false
        >(n->value, generator);

        return ret;
    }

    any CodeVisitor::visit(UnsignedLong* n)
    {
        auto ret = internal::integral_value<
            UnsignedLong::template_type,
            64,     // TODO: Magic number
            false
        >(n->value, generator);

        return ret;
    }

    any CodeVisitor::visit(Float* n)
    {
        auto ret = internal::fp_value<
            Float::template_type
        >(n->value, generator);

        return ret;
    }

    any CodeVisitor::visit(Double* n)
    {
        auto ret = internal::fp_value<
            Double::template_type
        >(n->value, generator);

        return ret;
    }

    any CodeVisitor::visit(Symbol* n)
    {
        auto ret = internal::integral_value<
            uint64_t,
            64,
            false
        >(util::symbol_hash(n->value), generator);

        return ret;
    }

    any CodeVisitor::visit(Nothing* n)
    {
        // Maybe change this, since some functions can return an actual nothing
        // instead of just being void.
        Value* ret = llvm::ConstantTokenNone::get(generator->context);

        return ret;
    }

    any CodeVisitor::visit(BinaryOp* n)
    {
        using ast::BinaryOperators;

        auto et = n->expression_type();
        auto as_simple = util::get_if<types::SimpleType>(&et);

        Value* lhs = util::any_cast<Value*>(n->left->visit(this));
        Value* rhs = util::any_cast<Value*>(n->right->visit(this));

        Value* ret = nullptr;

        if (as_simple != nullptr)
        {
            // Operations on simple types
            if (as_simple->is_integral)
            {
                // Operations on integer types
                switch (n->op)
                {
                    case BinaryOperators::Add:
                        ret = generator->builder.CreateAdd(lhs, rhs, "addtmp");
                        break;
                    case BinaryOperators::Subtract:
                        ret = generator->builder.CreateSub(lhs, rhs, "subtmp");
                        break;
                    case BinaryOperators::Multiply:
                        ret = generator->builder.CreateMul(lhs, rhs, "multmp");
                        break;
                    case BinaryOperators::Divide:
                        if (as_simple->type == BasicType::UnsignedByte ||
                            as_simple->type == BasicType::UnsignedInteger ||
                            as_simple->type == BasicType::UnsignedLong)
                        {
                            ret = generator->builder.CreateUDiv(lhs, rhs, "divtmp");
                        }
                        else
                        {
                            ret = generator->builder.CreateSDiv(lhs, rhs, "divtmp");
                        }
                        break;
                    case BinaryOperators::Modulus:
                        if (as_simple->type == BasicType::UnsignedByte ||
                            as_simple->type == BasicType::UnsignedInteger ||
                            as_simple->type == BasicType::UnsignedLong)
                        {
                            ret = generator->builder.CreateURem(lhs, rhs, "modtmp");
                        }
                        else
                        {
                            ret = generator->builder.CreateSRem(lhs, rhs, "modtmp");
                        }
                        break;
                    case BinaryOperators::Equals:
                        ret = generator->builder.CreateICmpEQ(lhs, rhs, "cmptmp");
                        break;
                    case BinaryOperators::NotEqual:
                        ret = generator->builder.CreateICmpNE(lhs, rhs, "cmptmp");
                        break;
                    // TODO: Others
                    default:
                        break;
                }
            }
            else if (as_simple->type == BasicType::Float || as_simple->type == BasicType::Double)
            {
                // Operations on floating-point types
                switch (n->op)
                {
                    case BinaryOperators::Add:
                        ret = generator->builder.CreateFAdd(lhs, rhs, "addtmp");
                        break;
                    case BinaryOperators::Subtract:
                        ret = generator->builder.CreateFSub(lhs, rhs, "subtmp");
                        break;
                    case BinaryOperators::Multiply:
                        ret = generator->builder.CreateFMul(lhs, rhs, "multmp");
                        break;
                    case BinaryOperators::Divide:
                        ret = generator->builder.CreateFDiv(lhs, rhs, "divtmp");
                        break;
                    case BinaryOperators::Modulus:
                        ret = generator->builder.CreateFRem(lhs, rhs, "modtmp");
                    case BinaryOperators::Equals:
                        ret = generator->builder.CreateFCmpEQ(lhs, rhs, "cmptmp");
                        break;
                    case BinaryOperators::NotEqual:
                        ret = generator->builder.CreateFCmpNE(lhs, rhs, "cmptmp");
                        break;
                    // TODO: Others
                    default:
                        break;
                }
            }
            else
            {
                // Operations on other simple types
            }
        }
        // TODO: Handle other types: structures, refs, pointers, etc.

        return ret;
    }
}}