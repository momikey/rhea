#include "inference/visitor.hpp"
#include "inference/engine.hpp"

namespace rhea { namespace inference {
    using namespace rhea::ast;
    using namespace rhea::types;
    using util::any;

    using type_vector = std::vector<TypeInfo>;

    // No need for return values in any of these.

    any InferenceVisitor::visit(Boolean* n)
    {
        engine->inferred_types[n] = InferredType { [](type_vector tv) { return SimpleType(BasicType::Boolean, false); } };
        return {};
    }

    any InferenceVisitor::visit(Integer* n)
    {
        engine->inferred_types[n] = InferredType { [](type_vector tv) { return SimpleType(BasicType::Integer, true, true); } };
        return {};
    }

    any InferenceVisitor::visit(Byte* n)
    {
        engine->inferred_types[n] = InferredType { [](type_vector tv) { return SimpleType(BasicType::Byte, true, true); } };
        return {};
    }

    any InferenceVisitor::visit(Long* n)
    {
        engine->inferred_types[n] = InferredType { [](type_vector tv) { return SimpleType(BasicType::Long, true, true); } };
        return {};
    }

    any InferenceVisitor::visit(UnsignedInteger* n)
    {
        engine->inferred_types[n] = InferredType { [](type_vector tv) { return SimpleType(BasicType::UnsignedInteger, true, true); } };
        return {};
    }

    any InferenceVisitor::visit(UnsignedByte* n)
    {
        engine->inferred_types[n] = InferredType { [](type_vector tv) { return SimpleType(BasicType::UnsignedByte, true, true); } };
        return {};
    }

    any InferenceVisitor::visit(UnsignedLong* n)
    {
        engine->inferred_types[n] = InferredType { [](type_vector tv) { return SimpleType(BasicType::UnsignedLong, true, true); } };
        return {};
    }

    any InferenceVisitor::visit(Float* n)
    {
        engine->inferred_types[n] = InferredType { [](type_vector tv) { return SimpleType(BasicType::Float, true, false); } };
        return {};
    }

    any InferenceVisitor::visit(Double* n)
    {
        engine->inferred_types[n] = InferredType { [](type_vector tv) { return SimpleType(BasicType::Double, true, false); } };
        return {};
    }

    any InferenceVisitor::visit(Symbol* n)
    {
        engine->inferred_types[n] = InferredType { [](type_vector tv) { return SimpleType(BasicType::Symbol, false); } };
        return {};
    }

    any InferenceVisitor::visit(Nothing* n)
    {
        engine->inferred_types[n] = InferredType { [](type_vector tv) { return NothingType(); } };
        return {};
    }

}}
