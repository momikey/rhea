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

    // Tests
    BOOST_AUTO_TEST_SUITE (AST_Binop)

    BOOST_AUTO_TEST_CASE (binop_ast)
    {
        ast::expression_ptr l = std::make_unique<ast::Integer>(42);
        ast::expression_ptr r = std::make_unique<ast::Integer>(69);
        auto op = ast::BinaryOperators::Add;

        auto node = std::make_unique<ast::BinaryOp>(op, l, r);

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        BOOST_TEST((node->op == ast::BinaryOperators::Add));

        auto node_l = dynamic_cast<ast::Integer*>(node->left.get());
        auto node_r = dynamic_cast<ast::Integer*>(node->right.get());
        BOOST_TEST(node_l->value + node_r->value == 42 + 69);
    }

    BOOST_AUTO_TEST_SUITE_END ()

    BOOST_AUTO_TEST_SUITE (AST_TernaryOp)

    BOOST_AUTO_TEST_CASE (ternary_ast)
    {
        ast::expression_ptr c = std::make_unique<ast::Integer>(42);
        ast::expression_ptr t = std::make_unique<ast::Boolean>(true);
        ast::expression_ptr f = std::make_unique<ast::Boolean>(false);

        auto node = std::make_unique<ast::TernaryOp>(c,t,f);

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        auto node_c = dynamic_cast<ast::Integer*>(node->condition.get());
        auto node_t = dynamic_cast<ast::Boolean*>(node->true_branch.get());
        auto node_f = dynamic_cast<ast::Boolean*>(node->false_branch.get());

        BOOST_TEST(node_c->value == 42);
        BOOST_TEST(node_t->value == !node_f->value);
    }

    BOOST_AUTO_TEST_SUITE_END ()
}