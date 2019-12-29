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
    BOOST_AUTO_TEST_SUITE (AST_modules)

    BOOST_AUTO_TEST_CASE (program_skeleton_ast)
    {
        ast::child_vector<ast::Statement> stmts;
        auto node = std::make_unique<ast::Program>(stmts);

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        BOOST_TEST((node->to_string() == "(Program)"));
    }

    BOOST_AUTO_TEST_SUITE_END ()
}