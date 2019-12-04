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
    BOOST_AUTO_TEST_SUITE (AST_simple_statements)

    BOOST_AUTO_TEST_CASE (bare_expression_ast)
    {
        ast::expression_ptr id = ast::make_identifier<ast::Identifier>("foo");

        auto node = std::make_unique<ast::BareExpression>(std::move(id));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        BOOST_TEST((node->to_string() == "(BareExpression,(Identifier,foo))"));
    }

    BOOST_AUTO_TEST_CASE (block_ast)
    {
        ast::expression_ptr id1 = ast::make_identifier<ast::Identifier>("foo");
        ast::expression_ptr id2 = ast::make_identifier<ast::Identifier>("bar");

        ast::child_vector<ast::Statement> stmts;
        stmts.emplace_back(ast::make_statement<ast::BareExpression>(std::move(id1)));
        stmts.emplace_back(ast::make_statement<ast::BareExpression>(std::move(id2)));

        auto node = std::make_unique<ast::Block>(stmts);

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        BOOST_TEST((node->to_string() == "(Block,(BareExpression,(Identifier,foo)),(BareExpression,(Identifier,bar)))"));
    }

    BOOST_AUTO_TEST_CASE (assign_ast)
    {
        ast::expression_ptr lhs = ast::make_identifier<ast::Identifier>("foo");
        ast::expression_ptr rhs = ast::make_expression<ast::Integer>(42);

        auto node = std::make_unique<ast::Assign>(std::move(lhs), std::move(rhs));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        BOOST_TEST((node->to_string() == "(Assign,(Identifier,foo),(Integral,42,0))"));
    }

    BOOST_AUTO_TEST_CASE (compound_assign_ast)
    {
        ast::expression_ptr lhs = ast::make_identifier<ast::Identifier>("foo");
        ast::expression_ptr rhs = ast::make_expression<ast::Integer>(42);

        auto node = std::make_unique<ast::CompoundAssign>(
            std::move(lhs),
            ast::AssignOperator::Add,
            std::move(rhs));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        BOOST_TEST((node->to_string() == "(CompoundAssign,(Identifier,foo),(Integral,42,0),0)"));
    }

    BOOST_AUTO_TEST_CASE (type_declaration_ast)
    {
        std::unique_ptr<ast::AnyIdentifier> lhs = ast::make_identifier<ast::Identifier>("foo");
        std::unique_ptr<ast::AnyIdentifier> tn = ast::make_identifier<ast::Identifier>("integer");
        auto rhs = std::make_unique<ast::Typename>(std::move(tn));

        auto node = std::make_unique<ast::TypeDeclaration>(std::move(lhs), std::move(rhs));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        BOOST_TEST((node->to_string() ==
            "(TypeDeclaration,(Identifier,foo),(Typename,(Identifier,integer),null,null))"));
    }

    BOOST_AUTO_TEST_CASE (variable_ast)
    {
        std::unique_ptr<ast::AnyIdentifier> lhs = ast::make_identifier<ast::Identifier>("foo");
        ast::expression_ptr rhs = ast::make_expression<ast::Integer>(42);

        auto node = std::make_unique<ast::Variable>(std::move(lhs), std::move(rhs));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        BOOST_TEST((node->to_string() == "(Variable,(Identifier,foo),(Integral,42,0))"));
    }

    BOOST_AUTO_TEST_CASE (constant_ast)
    {
        std::unique_ptr<ast::AnyIdentifier> lhs = ast::make_identifier<ast::Identifier>("foo");
        ast::expression_ptr rhs = ast::make_expression<ast::Integer>(42);

        auto node = std::make_unique<ast::Constant>(std::move(lhs), std::move(rhs));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        BOOST_TEST((node->to_string() == "(Constant,(Identifier,foo),(Integral,42,0))"));
    }

    BOOST_AUTO_TEST_CASE (do_statement_ast)
    {
        ast::expression_ptr id = ast::make_identifier<ast::Identifier>("foo");

        auto node = std::make_unique<ast::Do>(std::move(id));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        BOOST_TEST((node->to_string() == "(Do,(Identifier,foo))"));
    }

    BOOST_AUTO_TEST_SUITE_END ()
}