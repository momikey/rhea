#ifndef RHEA_CODEGEN_VISITOR_HPP
#define RHEA_CODEGEN_VISITOR_HPP

#include "../ast.hpp"
#include "../compat.hpp"
#include "../visitor/visitor.hpp"
#include "../visitor/default.hpp"
#include "generator_fwd.hpp"

#include <llvm/IR/Value.h>
#include <llvm/IR/Constant.h>

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
    };
}}

#endif /* RHEA_CODEGEN_VISITOR_HPP */