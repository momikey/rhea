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

    BOOST_AUTO_TEST_SUITE_END ()
}