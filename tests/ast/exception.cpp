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
    BOOST_AUTO_TEST_SUITE (AST_exception_statements)

    BOOST_AUTO_TEST_CASE (AST_throw_statement)
    {
        auto throw_expr = ast::make_expression<ast::Integer>(420);

        auto node = std::make_unique<ast::Throw>(std::move(throw_expr));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        BOOST_TEST((node->to_string() == "(Throw,(Integral,420,0))"));
    }

    BOOST_AUTO_TEST_CASE (AST_finally_statement)
    {
        auto lhs = std::make_unique<ast::Identifier>("foo");
        ast::expression_ptr rhs = ast::make_expression<ast::Integer>(42);
        auto assign = ast::make_statement<ast::Variable>(std::move(lhs), std::move(rhs));

        auto node = std::make_unique<ast::Finally>(std::move(assign));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        BOOST_TEST((node->to_string() == "(Finally,(Variable,(Identifier,foo),(Integral,42,0)))"));
    }

    BOOST_AUTO_TEST_CASE (AST_catch_statement)
    {
        auto throw_expr = ast::make_expression<ast::Integer>(420);
        auto throw_stmt = ast::make_statement<ast::Throw>(std::move(throw_expr));

        auto type_ident = ast::make_identifier<ast::Identifier>("integer");
        auto type = std::make_unique<ast::Typename>(std::move(type_ident));
        auto type_pair = std::make_unique<ast::TypePair>("e", std::move(type));

        auto node = std::make_unique<ast::Catch>(std::move(type_pair), std::move(throw_stmt));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        BOOST_TEST((node->to_string() ==
            "(Catch,(TypePair,e,(Typename,(Identifier,integer),null,null)),(Throw,(Integral,420,0)))"));
    }

    BOOST_AUTO_TEST_CASE (AST_try_statement)
    {
        auto throw_expr = ast::make_expression<ast::Integer>(420);
        auto throw_stmt = ast::make_statement<ast::Throw>(std::move(throw_expr));

        auto type_ident = ast::make_identifier<ast::Identifier>("integer");
        auto type = std::make_unique<ast::Typename>(std::move(type_ident));
        auto type_pair = std::make_unique<ast::TypePair>("e", std::move(type));

        auto catch_stmt = std::make_unique<ast::Catch>(std::move(type_pair), std::move(throw_stmt));

        auto lhs = std::make_unique<ast::Identifier>("foo");
        ast::expression_ptr rhs = ast::make_expression<ast::Integer>(42);
        auto assign = ast::make_statement<ast::Variable>(std::move(lhs), std::move(rhs));

        auto finally_stmt = std::make_unique<ast::Finally>(std::move(assign));

        ast::child_vector<ast::Catch> catches;
        catches.push_back(std::move(catch_stmt));

        throw_expr = ast::make_expression<ast::Integer>(420);
        throw_stmt = ast::make_statement<ast::Throw>(std::move(throw_expr));
        auto node = std::make_unique<ast::Try>(std::move(throw_stmt), catches, std::move(finally_stmt));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        BOOST_TEST((node->to_string() ==
            "(Try,(Throw,(Integral,420,0)),(Finally,(Variable,(Identifier,foo),(Integral,42,0))),(Catch,(TypePair,e,(Typename,(Identifier,integer),null,null)),(Throw,(Integral,420,0))))"));
    }

    BOOST_AUTO_TEST_SUITE_END ()
}