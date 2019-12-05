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
    std::string container_sample_data[] = { "foo", "bar", "xyz" };

    // Test cases
    BOOST_AUTO_TEST_SUITE (AST_Structure_Types)

    BOOST_AUTO_TEST_CASE (array_ast)
    {
        ast::child_vector<ast::Expression> items;
        for (auto&& i : container_sample_data)
        {
            ast::expression_ptr p = ast::make_expression<ast::String>(i);
            items.emplace_back(std::move(p));
        }

        auto node = std::make_unique<ast::Array>(items);

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        // Boost.Test uses macros, which don't like escaped quotes.
        const std::string result = "(Array,(String,\"foo\"),(String,\"bar\"),(String,\"xyz\"))";
        BOOST_TEST_MESSAGE("Target " << result);

        BOOST_TEST((node->to_string() == result));
    }

    BOOST_AUTO_TEST_CASE (list_ast)
    {
        ast::child_vector<ast::Expression> items;
        for (auto&& i : container_sample_data)
        {
            ast::expression_ptr p = ast::make_expression<ast::String>(i);
            items.emplace_back(std::move(p));
        }

        auto node = std::make_unique<ast::List>(items);

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        // Boost.Test uses macros, which don't like escaped quotes.
        const std::string result = "(List,(String,\"foo\"),(String,\"bar\"),(String,\"xyz\"))";
        BOOST_TEST_MESSAGE("Target " << result);

        BOOST_TEST((node->to_string() == result));
    }

    BOOST_AUTO_TEST_CASE (tuple_ast)
    {
        ast::child_vector<ast::Expression> items;
        for (auto&& i : container_sample_data)
        {
            ast::expression_ptr p = ast::make_expression<ast::String>(i);
            items.emplace_back(std::move(p));
        }

        auto node = std::make_unique<ast::Tuple>(items);

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        // Boost.Test uses macros, which don't like escaped quotes.
        const std::string result = "(Tuple,(String,\"foo\"),(String,\"bar\"),(String,\"xyz\"))";
        BOOST_TEST_MESSAGE("Target " << result);

        BOOST_TEST((node->to_string() == result));
    }

    BOOST_AUTO_TEST_SUITE_END ()
}