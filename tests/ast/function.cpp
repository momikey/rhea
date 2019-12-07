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
    BOOST_AUTO_TEST_SUITE (AST_function)

    BOOST_AUTO_TEST_CASE (call_empty_ast)
    {
        auto fname = ast::make_identifier<ast::Identifier>("f");
        auto node = std::make_unique<ast::Call>(std::move(fname));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        BOOST_TEST((node->to_string() == "(Call,(Identifier,f))"));
    }

    BOOST_AUTO_TEST_CASE (call_positional_ast)
    {
        auto fname = ast::make_identifier<ast::Identifier>("f");
        auto arg1 = ast::make_expression<ast::Integer>(42);
        auto arg2 = ast::make_expression<ast::String>("foo");

        ast::child_vector<ast::Expression> args;
        args.emplace_back(std::move(arg1));
        args.emplace_back(std::move(arg2));

        auto node = std::make_unique<ast::Call>(std::move(fname), args);

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        auto target = "(Call,(Identifier,f),(Integral,42,0),(String,\"foo\"))";
        BOOST_TEST((node->to_string() == target));
    }

    BOOST_AUTO_TEST_CASE (call_named_ast)
    {
        auto fname = ast::make_identifier<ast::Identifier>("f");

        auto arg1 = ast::make_expression<ast::Integer>(42);
        auto arg2 = ast::make_expression<ast::String>("foo");

        auto named1 = std::make_unique<ast::NamedArgument>("a", std::move(arg1));
        auto named2 = std::make_unique<ast::NamedArgument>("b", std::move(arg2));

        ast::child_vector<ast::NamedArgument> args;
        args.emplace_back(std::move(named1));
        args.emplace_back(std::move(named2));

        auto node = std::make_unique<ast::Call>(std::move(fname), args);
        BOOST_TEST((node->arguments.size() == 2));
        BOOST_TEST((node->arguments[0].index() == 1));
        auto a1 = boost::variant2::get<1>(node->arguments[0]).get();
        BOOST_TEST((a1->name == "a"));
        // auto a1 = boost::variant2::get_if<std::unique_ptr<ast::NamedArgument>>(&(node->arguments[0]));
        // BOOST_TEST((a1 != nullptr));
        
        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        // auto target = "(Call,(Identifier,f),(NamedArgument,a,(Integral,42,0)),(NamedArgument,b,(String,\"foo\")))";
        // BOOST_TEST((node->to_string() == target));
    }

    BOOST_AUTO_TEST_SUITE_END ()
}