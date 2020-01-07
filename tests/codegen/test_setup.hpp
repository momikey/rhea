#ifndef RHEA_TEST_CODEGEN_SETUP_HPP
#define RHEA_TEST_CODEGEN_SETUP_HPP

/*
 * Setup for the codegen testing module.
 */

#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/BasicBlock.h>

#include "../../include/codegen/generator.hpp"

namespace rhea { namespace codegen {
    /*
     * Wrapper to put a bare expression into an LLVM function, for debugging purposes.
     */
    llvm::Function* wrap_expression(llvm::Value* expr, CodeGenerator* gen);
}}

#endif /* RHEA_TEST_CODEGEN_SETUP_HPP */