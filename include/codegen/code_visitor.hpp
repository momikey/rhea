#ifndef RHEA_CODEGEN_VISITOR_HPP
#define RHEA_CODEGEN_VISITOR_HPP

#include <llvm/IR/Value.h>
#include <llvm/IR/Constants.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/APFloat.h>

#include "../ast.hpp"
#include "../util/compat.hpp"
#include "../util/symbol_hash.hpp"
#include "../visitor/visitor.hpp"
#include "../visitor/default.hpp"
#include "generator_fwd.hpp"

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
        any visit(BinaryOp* n) override;
    };
}}

#endif /* RHEA_CODEGEN_VISITOR_HPP */