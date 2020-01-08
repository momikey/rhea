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
    std::array<std::unique_ptr<ast::ASTNode>, 6> integral_samples = {
        std::make_unique<ast::Integer>(-42),
        std::make_unique<ast::Byte>(127),
        std::make_unique<ast::Long>(1L << 32),
        std::make_unique<ast::UnsignedInteger>(9999999),
        std::make_unique<ast::UnsignedByte>(255),
        std::make_unique<ast::UnsignedLong>((1L << 63) + 1)
    };

    std::array<std::unique_ptr<ast::ASTNode>, 7> fp_samples = {
        std::make_unique<ast::Float>(1.0f),
        std::make_unique<ast::Float>(-0.007f),
        std::make_unique<ast::Float>(1e-6f),
        std::make_unique<ast::Double>(0.0),
        std::make_unique<ast::Double>(1e100),
        std::make_unique<ast::Double>(-1e-100),
        std::make_unique<ast::Double>(1.234567890987654321)
    };

    std::array<std::unique_ptr<ast::Symbol>, 5> symbol_samples = {
        std::make_unique<ast::Symbol>("foo"),
        std::make_unique<ast::Symbol>("Foo"),
        std::make_unique<ast::Symbol>("bar"),
        std::make_unique<ast::Symbol>("a1b2c3"),
        std::make_unique<ast::Symbol>("really_LONG_symbol_name_with_numbers")
    };

    template <typename T>
    auto samples_to_vector(T& vec)
    {
        std::vector<ast::ASTNode*> v;
        for (auto& p : vec)
        {
            v.push_back(p.get());
        }
        return v;
    }

    // Test cases
    BOOST_FIXTURE_TEST_SUITE (codegen_literals, CodegenFixture)

    BOOST_AUTO_TEST_CASE (cg_boolean_literal)
    {
        std::unique_ptr<ast::ASTNode> node = std::make_unique<ast::Boolean>(true);

        BOOST_TEST_MESSAGE("Codegen for boolean literal " << node->to_string());

        auto result = util::any_cast<llvm::Value*>(gen.generate(node.get()));

        BOOST_TEST((result != nullptr));
        result->print(llvm::outs(), true);
    }

    BOOST_AUTO_TEST_CASE (cg_nothing_literal)
    {
        std::unique_ptr<ast::ASTNode> node = std::make_unique<ast::Nothing>();

        BOOST_TEST_MESSAGE("Codegen for nothing literal " << node->to_string());

        auto result = util::any_cast<llvm::Value*>(gen.generate(node.get()));

        BOOST_TEST((result != nullptr));
        result->print(llvm::outs(), true);
    }

    BOOST_DATA_TEST_CASE (cg_integer_literal, data::make(samples_to_vector(integral_samples)))
    {
        BOOST_TEST_MESSAGE("Codegen for integer literal " << sample->to_string());

        auto result = util::any_cast<llvm::Value*>(gen.generate(sample));

        BOOST_TEST((result != nullptr));
        result->print(llvm::outs(), true);
    }

    BOOST_DATA_TEST_CASE (cg_fp_literal, data::make(samples_to_vector(fp_samples)))
    {
        BOOST_TEST_MESSAGE("Codegen for floating-point literal " << sample->to_string());

        auto result = util::any_cast<llvm::Value*>(gen.generate(sample));

        BOOST_TEST((result != nullptr));
        result->print(llvm::outs(), true);
    }

    BOOST_DATA_TEST_CASE (cg_symbol_literal, data::make(samples_to_vector(symbol_samples)))
    {
        BOOST_TEST_MESSAGE("Codegen for symbol " << sample->to_string());

        auto result = util::any_cast<llvm::Value*>(gen.generate(sample));

        BOOST_TEST((result != nullptr));
        result->print(llvm::outs(), true);
    }

    BOOST_AUTO_TEST_SUITE_END ()
}