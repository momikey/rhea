#ifndef RHEA_CODEGEN_GENERATOR_HPP
#define RHEA_CODEGEN_GENERATOR_HPP

#include <memory>
#include <string>

#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Passes/PassBuilder.h>
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

#include "../ast.hpp"
#include "../state/symbol.hpp"
#include "../types/types.hpp"
#include "../types/mapper.hpp"
#include "../util/compat.hpp"
#include "code_visitor.hpp"
#include "allocation_manager.hpp"

/*
 * The core class for Rhea code generation using LLVM.
 */
namespace rhea { namespace codegen {
    using util::any;
    using namespace ast;

    // Forward declaration for helper class.
    struct TypeBuilder;

    struct CodeGenerator
    {
        CodeGenerator();
        CodeGenerator(std::string module);

        ////
        // Rhea-specific members: these can be public
        ////
        CodeVisitor visitor;
        state::ScopeManager scope_manager;
        AllocationManager allocation_manager;
        types::TypeMapper type_mapper;

        // Make our visitor a friend class, so it can access all the LLVM parts.
        friend CodeVisitor;

        // At present, this returns a std::any (C++17( or boost::any (C++14).
        // This may change in the future.
        util::any generate(ast::ASTNode* tree);

        // Run optimization passes over a section of IR code.
        // TODO: Find some way to let callers determine which passes to use.
        template <typename Code>
        Code* optimize(Code* code);

        ////
        // Public LLVM members
        ////

        // The LLVM context
        llvm::LLVMContext context;

        // IR builder, for easier generation of most IR code
        llvm::IRBuilder<> builder;

        // Pointer to the LLVM module which will receive our code.
        // Later on, we may need more than just one, but we can start small.
        std::unique_ptr<llvm::Module> module;

        // The target machine
        llvm::TargetMachine* target_machine;

        ////
        // Helper methods
        ////

        // Get the LLVM IR type for a Rhea type.
        llvm::Type* llvm_for_type(types::TypeInfo ti);

        // Push a new declaration and allocation scope.
        void create_scope(std::string name);

        // Destroy the current scope.
        void destroy_scope();

        private:
        ////
        // Private LLVM members
        ////

        // Set up our general optimization passes
        void initialize_passes();

        // Do some module init stuff, like creating an initial function
        void initialize_module();

        // Close out the initial function
        void finalize_module();

        // Manager for function-level optimization
        llvm::FunctionPassManager FPM;
        llvm::FunctionAnalysisManager FAM;
    };

    // Helper class to map Rhea types to those used in LLVM IR.
    struct TypeBuilder
    {
        CodeGenerator* generator;

        // We use the "overloaded operator()" pattern for this one.
        template <typename T>
        llvm::Type* operator()(T t) { return nullptr; }
    };

}}

#endif /* RHEA_CODEGEN_GENERATOR_HPP */