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

#include "test_setup.hpp"

namespace data = boost::unit_test::data;
namespace ast = rhea::ast;
namespace cg = rhea::codegen;
namespace util = rhea::util;

namespace {
    struct CodegenFixture
    {
        cg::CodeGenerator gen;
    };

    // Datasets

    // Test cases
    BOOST_FIXTURE_TEST_SUITE (codegen_literals, CodegenFixture)

    BOOST_AUTO_TEST_CASE (cg_binary_op)
    {
        ast::expression_ptr l = std::make_unique<ast::Integer>(42);
        ast::expression_ptr r = std::make_unique<ast::Integer>(69);
        auto op = ast::BinaryOperators::Add;

        auto node = std::make_unique<ast::BinaryOp>(op, std::move(l), std::move(r));

        BOOST_TEST_MESSAGE("Codegen for binary op " << node->to_string());

        auto result = util::any_cast<llvm::Value*>(gen.generate(node.get()));

        BOOST_TEST((result != nullptr));
        result->print(llvm::outs(), true);
    }

    BOOST_AUTO_TEST_SUITE_END ()
}