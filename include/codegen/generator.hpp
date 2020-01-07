#ifndef RHEA_CODEGEN_GENERATOR_HPP
#define RHEA_CODEGEN_GENERATOR_HPP

#include <memory>
#include <string>

#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>

#include "../ast.hpp"
#include "code_visitor.hpp"

/*
 * The core class for Rhea code generation using LLVM.
 */
namespace rhea { namespace codegen {
    using util::any;
    using namespace ast;

    struct CodeGenerator
    {
        CodeGenerator() : context(), builder(context), 
            module(std::make_unique<llvm::Module>("main", context)), visitor(this)
        {}
        CodeGenerator(std::string module) : context(), builder(context),
            module(std::make_unique<llvm::Module>(module, context)), visitor(this)
        {}

        llvm::LLVMContext context;
        llvm::IRBuilder<> builder;
        std::unique_ptr<llvm::Module> module;
        CodeVisitor visitor;

        // Do we need this? This class probably won't have any private members,
        // but we might want to keep it for clarity, if nothing else.
        friend CodeVisitor;

        // At present, this returns a std::any (C++17( or boost::any (C++14).
        // This may change in the future.
        auto generate(ast::ASTNode* tree) { return tree->visit(&visitor); }
    };
}}

#endif /* RHEA_CODEGEN_GENERATOR_HPP */