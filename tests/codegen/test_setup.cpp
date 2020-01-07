#include "test_setup.hpp"

namespace rhea { namespace codegen {
    /*
     * Wrapper to put a bare expression into an LLVM function, for debugging purposes.
     */
    llvm::Function* wrap_expression(llvm::Value* expr, CodeGenerator* gen)
    {
        auto ft = llvm::FunctionType::get(
            expr->getType(),
            false
        );

        auto fn = llvm::Function::Create(
            ft,
            llvm::Function::ExternalLinkage,
            "anonymous",
            gen->module.get()
        );

        auto block = llvm::BasicBlock::Create(gen->context, "entry", fn);
        gen->builder.SetInsertPoint(block);

        gen->builder.CreateRet(expr);
        return fn;
    }
}}