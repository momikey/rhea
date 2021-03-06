#ifndef RHEA_CODEGEN_VISITOR_HPP
#define RHEA_CODEGEN_VISITOR_HPP

#include <string>
#include <memory>

#include <llvm/IR/Value.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/APFloat.h>

#include <fmt/format.h>

#include "../ast.hpp"
#include "../types/to_string.hpp"
#include "../util/compat.hpp"
#include "../util/symbol_hash.hpp"
#include "../visitor/visitor.hpp"
#include "../visitor/default.hpp"
#include "generator_fwd.hpp"
#include "type_convert.hpp"

/*
 * The AST visitor for the code generation pass of the Rhea compiler.
 */
namespace rhea { namespace codegen {
    using util::any;
    using namespace rhea::ast;

    struct CodeVisitor : visitor::DefaultVisitor
    {
        CodeVisitor(CodeGenerator* g) : generator(g) {}
        CodeGenerator* generator = nullptr;

        // Code generation visitors for AST nodes.
        // Only those nodes which generate code should be here.
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
        any visit(Identifier* n) override;
        any visit(BinaryOp* n) override;
        any visit(UnaryOp* n) override;
        any visit(TernaryOp* n) override;

        any visit(If* n) override;
        any visit(BareExpression* n) override;
        any visit(Block* n) override;
        any visit(TypeDeclaration* n) override;
        any visit(Variable* n) override;
        any visit(Constant* n) override;
    };
}}

#endif /* RHEA_CODEGEN_VISITOR_HPP */