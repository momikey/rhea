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

    BOOST_AUTO_TEST_CASE (dictionary_entry_ast)
    {
        auto key = std::make_unique<ast::Symbol>("foo");
        auto value = ast::make_expression<ast::Boolean>(true);

        auto node = std::make_unique<ast::DictionaryEntry>(std::move(key), std::move(value));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        // Boost.Test uses macros, which don't like escaped quotes.
        const std::string result = "(DictionaryEntry,(Symbol,foo),(Boolean,true))";
        BOOST_TEST_MESSAGE("Target " << result);

        BOOST_TEST((node->to_string() == result));
    }

    BOOST_AUTO_TEST_CASE (dictionary_ast)
    {
        auto k1 = std::make_unique<ast::Symbol>("foo");
        auto v1 = ast::make_expression<ast::Boolean>(true);

        auto k2 = std::make_unique<ast::String>("bar");
        auto v2 = ast::make_expression<ast::Boolean>(false);

        ast::child_vector<ast::DictionaryEntry> entries;
        entries.push_back(std::move(std::make_unique<ast::DictionaryEntry>(std::move(k1), std::move(v1))));
        entries.push_back(std::move(std::make_unique<ast::DictionaryEntry>(std::move(k2), std::move(v2))));

        auto node = std::make_unique<ast::Dictionary>(entries);

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());
        // Boost.Test uses macros, which don't like escaped quotes.
        const std::string result = "(Dictionary,(DictionaryEntry,(Symbol,foo),(Boolean,true)),(DictionaryEntry,(String,\"bar\"),(Boolean,false)))";
        BOOST_TEST_MESSAGE("Target " << result);

        BOOST_TEST((node->to_string() == result));
    }

    BOOST_AUTO_TEST_CASE (structure_declaration_ast)
    {
        std::vector<std::string> ns = { "foo", "bar" };

        ast::child_vector<ast::TypePair> fields;
        auto f0 = std::make_unique<ast::TypePair>(
            ns[0],
            std::move(std::make_unique<ast::Typename>(std::move(ast::make_identifier<ast::Identifier>("integer"))))
        );
        auto f1 = std::make_unique<ast::TypePair>(
            ns[1],
            std::move(std::make_unique<ast::Typename>(std::move(ast::make_identifier<ast::Identifier>("string"))))
        );
        fields.emplace_back(std::move(f0));
        fields.emplace_back(std::move(f1));

        auto name = std::make_unique<ast::Identifier>("Test");

        auto node = std::make_unique<ast::Structure>(std::move(name), fields);

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());
        BOOST_TEST((node->to_string() ==
            "(Structure,(Identifier,Test),(TypePair,foo,(Typename,(Identifier,integer),null,null)),(TypePair,bar,(Typename,(Identifier,string),null,null)))"
        ));
    }

    BOOST_AUTO_TEST_SUITE_END ()
}