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
    BOOST_AUTO_TEST_SUITE (AST_Typenames)

    BOOST_AUTO_TEST_CASE (simple_typename)
    {
        std::unique_ptr<ast::AnyIdentifier> name = std::make_unique<ast::Identifier>("Foo");

        auto node = std::make_unique<ast::Typename>(name);

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());
        BOOST_TEST((node->name->to_string() == "(Identifier,Foo)"));
        BOOST_TEST((node->generic_part == nullptr));
        BOOST_TEST((node->array_part == nullptr));
    }

    BOOST_AUTO_TEST_CASE (generic_typenames)
    {
        auto name = ast::make_identifier<ast::Identifier>("Foo");
        auto gname = ast::make_identifier<ast::Identifier>("Ty");
        ast::child_vector<ast::Typename> gens;
        gens.emplace_back(std::make_unique<ast::Typename>(gname));

        auto gnode = std::make_unique<ast::GenericTypename>(gens);
        auto node = std::make_unique<ast::Typename>(name, gnode);

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());
        BOOST_TEST((node->name->to_string() == "(Identifier,Foo)"));
        BOOST_TEST((node->generic_part->to_string() == "(GenericTypename,(Typename,(Identifier,Ty),null,null))"));
        BOOST_TEST((node->array_part == nullptr));
    }

    BOOST_AUTO_TEST_CASE (array_typenames)
    {
        auto name = ast::make_identifier<ast::Identifier>("Foo");
        auto array = ast::make_expression<ast::Integer>(42);

        auto node = std::make_unique<ast::Typename>(name, array);

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());
        BOOST_TEST((node->name->to_string() == "(Identifier,Foo)"));
        BOOST_TEST((node->generic_part == nullptr));
        BOOST_TEST((node->array_part->to_string() == "(Integral,42,0)"));
    }

    BOOST_AUTO_TEST_SUITE_END ()
}