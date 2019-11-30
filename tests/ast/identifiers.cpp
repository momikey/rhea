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

namespace {
    // Datasets
    std::string identifier_samples[] = { "a", "foo", "a_long_identifier", "With42Numbers" };
    std::string fully_qualified_samples[] =
        { "a:b", "foo:bar:baz", "Big:Long:List:of:Module:names", "Now:With1:Number___" };

    BOOST_AUTO_TEST_SUITE (AST_Identifiers)

    BOOST_DATA_TEST_CASE(identifier_ast, data::make(identifier_samples))
    {
        auto node = rhea::ast::Identifier(sample);

        BOOST_TEST_MESSAGE("Testing AST Node " << node.to_string());

        BOOST_TEST(node.name == sample);
    }

    BOOST_DATA_TEST_CASE(fully_qualified_ast, data::make(fully_qualified_samples))
    {
        std::vector<std::string> ids;
        boost::algorithm::split(ids, sample, [] (auto e) { return e == ':'; });

        auto node = rhea::ast::FullyQualified(ids);

        BOOST_TEST_MESSAGE("Testing AST Node " << node.to_string());

        BOOST_TEST(node.children().size() == ids.size());
    }

    BOOST_DATA_TEST_CASE(fully_qualified_ast_2, data::make(fully_qualified_samples))
    {
        std::vector<std::string> ids;
        boost::algorithm::split(ids, sample, [] (auto e) { return e == ':'; });

        std::vector<std::unique_ptr<rhea::ast::Identifier>> nodes;

        for (auto&& id : ids)
        {
            nodes.emplace_back(std::make_unique<rhea::ast::Identifier>(id));
        }

        auto node = rhea::ast::FullyQualified(nodes);

        BOOST_TEST_MESSAGE("Testing AST Node " << node.to_string());

        BOOST_TEST(node.children().size() == ids.size());
        for (auto i = 0u; i < node.children().size(); ++i)
        {
            BOOST_TEST(node.children()[i]->name == ids[i]);
        }
    }

    BOOST_AUTO_TEST_SUITE_END ()
}