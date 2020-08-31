#ifndef RHEA_DEBUG_BUILD_ASM_HPP
#define RHEA_DEBUG_BUILD_ASM_HPP

#include <memory>
#include <iostream>

#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Function.h>

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/SmallString.h>

#include "../ast.hpp"
#include "../codegen/generator.hpp"
#include "../util/compat.hpp"

namespace rhea { namespace debug {
    void print_asm(ast::ASTNode* tree)
    {
        // This is static so we can add to it with each successive calll.
        static codegen::CodeGenerator generator("debug");

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
        auto result = generator.generate(node);

        llvm::SmallString<0> output;
        llvm::raw_svector_ostream ostr(output);

        llvm::legacy::PassManager pm;

        if (generator.target_machine->addPassesToEmitFile(
            pm,
            ostr,
            nullptr,
            llvm::TargetMachine::CGFT_AssemblyFile,
            false
        ))
        {
            std::cerr << "Unable to write assembly\n";
        }
        else
        {
            pm.run(*(generator.module));
            std::clog << "Output: " << '\n' << output.str().str() << '\n';
        }
    }
}}

#endif /* RHEA_DEBUG_BUILD_ASM_HPP */