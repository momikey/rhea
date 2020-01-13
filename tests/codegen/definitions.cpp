#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <string>
#include <memory>
#include <vector>
#include <array>
#include <iostream>

#include "../../include/codegen/generator.hpp"
#include "../../include/codegen/code_visitor.hpp"
#include "../../include/ast.hpp"

#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/raw_ostream.h>

#include "test_setup.hpp"

namespace data = boost::unit_test::data;
namespace ast = rhea::ast;
namespace cg = rhea::codegen;
namespace util = rhea::util;

namespace {
    struct CodegenFixture
    {
        cg::CodeGenerator gen;

        void setup()
        {
            // auto ft = llvm::FunctionType::get(
            //     llvm::Type::getVoidTy(gen.context),
            //     false
            // );

            // auto fn = llvm::Function::Create(
            //     ft,
            //     llvm::Function::ExternalLinkage,
            //     "anonymous",
            //     gen.module.get()
            // );

            // auto block = llvm::BasicBlock::Create(gen.context, "entry", fn);
            // gen.builder.SetInsertPoint(block);
        }

        void teardown()
        {
            gen.module->print(llvm::outs(), nullptr, false, true);
        }
    };

    // Datasets

    // Test cases
    BOOST_FIXTURE_TEST_SUITE (codegen_definitions, CodegenFixture)

    BOOST_AUTO_TEST_CASE (codegen_variable_declaration)
    {
        auto lhs = std::make_unique<ast::Identifier>("foo");
        auto rhs = ast::make_expression<ast::Integer>(42);

        auto node = std::make_unique<ast::Variable>(std::move(lhs), std::move(rhs));

        BOOST_TEST_MESSAGE("Codegen for variable declaration " << node->to_string());

        auto aresult = gen.generate(node.get());

        // auto result = util::any_cast<llvm::Function*>(aresult);
        // BOOST_TEST((result != nullptr));
        
        BOOST_TEST((gen.scope_manager.is_local("foo")));
        // BOOST_TEST((gen.allocation_manager.is_local("foo")));

        // auto ai = gen.allocation_manager.find("foo");
        // BOOST_TEST((ai.has_value()));
        // BOOST_TEST((ai.value() != nullptr));
    }

    BOOST_AUTO_TEST_SUITE_END ()
}