#ifndef RHEA_INFERENCE_VISITOR_HPP
#define RHEA_INFERENCE_VISITOR_HPP

#include "ast.hpp"
#include "visitor/visitor.hpp"
#include "visitor/default.hpp"
#include "util/compat.hpp"

#include "lazy_type.hpp"
#include "engine_fwd.hpp"

/*
 * The type inference engine needs a visitor, too. This uses the same
 * double dispatch pattern as codegen, but it's geared toward building
 * the map of inferred types for each part of the AST.
 */
namespace rhea { namespace inference {
    using namespace rhea::ast;
    using util::any;

    struct InferenceVisitor : visitor::DefaultVisitor
    {
        InferenceVisitor(TypeEngine* e) : engine(e) {}

        TypeEngine* engine = nullptr;

        // We'll eventually override pretty much all the node-specific methods.
        any visit(Boolean* n) override;
        any visit(Integer* n) override;
        any visit(Byte* n) override;
        any visit(Long* n) override;
        any visit(UnsignedInteger* n) override;
        any visit(UnsignedByte* n) override;
        any visit(UnsignedLong* n) override;
        any visit(Float* n) override;
        any visit(Double* n) override;
        any visit(Symbol* n) override;
        any visit(Nothing* n) override;

        any visit(BinaryOp* n) override;
        any visit(UnaryOp* n) override;
        any visit(TernaryOp* n) override;

        any visit(Typename*) override;
        any visit(Variant*) override;
        any visit(Optional*) override;
        any visit(Cast*) override;
        any visit(TypeCheck*) override;
        any visit(Alias*) override;
        any visit(Enum*) override;
        any visit(SymbolList*) override;
        
        any visit(If* n) override;
        any visit(BareExpression* n) override;
        any visit(Block* n) override;
        any visit(While*) override;
        any visit(For*) override;
        any visit(With*) override;
        any visit(Break*) override;
        any visit(Continue*) override;
        any visit(Match*) override;
        any visit(On*) override;
        any visit(When*) override;
        any visit(TypeCase*) override;
        any visit(Default*) override;
        any visit(PredicateCall*) override;
        any visit(TypeDeclaration* n) override;
        any visit(Variable* n) override;
        any visit(Constant* n) override;
    };
}}

#endif /* RHEA_INFERENCE_VISITOR_HPP */