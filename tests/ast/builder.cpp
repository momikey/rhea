#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/algorithm/string/split.hpp>

#include <string>
#include <vector>
#include <memory>

#include <tao/pegtl.hpp>

#include "../../include/ast.hpp"
#include "../../include/grammar.hpp"

#include "test_setup.hpp"

namespace data = boost::unit_test::data;
using tao::pegtl::parse;
using tao::pegtl::string_input;
namespace pt = tao::pegtl::parse_tree;
namespace gr = rhea::grammar;
namespace ast = rhea::ast;

namespace {
    template <typename GrammarNode>
    std::unique_ptr<ast::parser_node> tree_builder(string_input<>& in)
    {
        return pt::parse<
            GrammarNode,
            ast::parser_node,
            ast::tree_selector
        >(in);
    }

    // Datasets

    // Test cases
    BOOST_AUTO_TEST_SUITE (AST_builder)

    BOOST_AUTO_TEST_CASE (builder_bare_expression)
    {
        std::string sample { "1.23;" };

        BOOST_TEST_MESSAGE("Parsing bare expression " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::bare_expression>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST((node->to_string() == "(BareExpression,(FloatingPoint,1.23,3))"));
    }

    BOOST_AUTO_TEST_SUITE_END ()
}