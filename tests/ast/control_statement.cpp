#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/algorithm/string/split.hpp>

#include <string>
#include <vector>
#include <memory>

#include "../../include/ast.hpp"

#include "test_setup.hpp"

namespace data = boost::unit_test::data;
namespace ast = rhea::ast;

namespace {
    // Datasets

    // Test cases
    BOOST_AUTO_TEST_SUITE (AST_control_statements)

    BOOST_AUTO_TEST_CASE (if_statement_ast_null)
    {
        ast::expression_ptr cond = ast::make_identifier<ast::Identifier>("foo");

        auto node = std::make_unique<ast::If>(
            std::move(cond),
            nullptr,
            nullptr);

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        BOOST_TEST((node->to_string() == "(If,(Identifier,foo),null,null)"));
    }

    BOOST_AUTO_TEST_SUITE_END ()
}