#ifndef RHEA_DEBUG_BUILD_IR_HPP
#define RHEA_DEBUG_BUILD_IR_HPP

#include <memory>
#include <iostream>

#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Function.h>

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/raw_ostream.h>

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


        std::string output;

        auto rsos = std::make_unique<llvm::raw_string_ostream>(output);
        auto buf = std::make_unique<llvm::buffer_ostream>(*rsos);

        llvm::legacy::PassManager pm;

        if (irp.generator.target_machine->addPassesToEmitFile(
            pm,
            *buf,
            nullptr,
            llvm::TargetMachine::CGFT_AssemblyFile
        ))
        {
            std::cerr << "Unable to write assembly\n";
        }
        else
        {
            pm.run(*(irp.generator.module));

            std::clog << "Output: " << output.length() << '\n' << output << '\n';
        }
    }
}}

#endif /* RHEA_DEBUG_BUILD_IR_HPP */