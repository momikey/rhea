#ifndef RHEA_DEBUG_BUILD_IR_HPP
#define RHEA_DEBUG_BUILD_IR_HPP

#include <memory>
#include <iostream>

#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Function.h>

#include "../ast.hpp"
#include "../codegen/generator.hpp"
#include "../util/compat.hpp"

namespace rhea { namespace debug {
    namespace internal {
        struct IRPrinter
        {
            IRPrinter() : generator("debug") {}
            codegen::CodeGenerator generator;
        };
    }

    void print_ir(ast::ASTNode* tree)
    {
        // codegen::CodeGenerator generator("debug");
        static internal::IRPrinter irp {};

        auto root = dynamic_cast<ast::Program*>(tree);

        ast::ASTNode* node = nullptr;
        if (root == nullptr)
        {
            node = tree;
        }
        else
        {
            node = root->children.front().get();
        }
        auto result = irp.generator.generate(node);
        irp.generator.module->print(llvm::outs(), nullptr, false, true);
    }
}}

#endif /* RHEA_DEBUG_BUILD_IR_HPP */