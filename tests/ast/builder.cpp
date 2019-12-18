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

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(BareExpression,(FloatingPoint,1.23,3))"));
    }

    BOOST_AUTO_TEST_CASE (builder_binop_expression)
    {
        std::string sample { "42 + 24;" };

        BOOST_TEST_MESSAGE("Parsing binary operation " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::bare_expression>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(BareExpression,(BinaryOp,0,(Integral,42,0),(Integral,24,0)))"));
    }

    BOOST_AUTO_TEST_CASE (builder_unaryop_expression)
    {
        std::string sample { "not x;" };

        BOOST_TEST_MESSAGE("Parsing unary operation " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::bare_expression>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(BareExpression,(UnaryOp,2,(Identifier,x)))"));
    }

    BOOST_AUTO_TEST_CASE (builder_literal_expressions)
    {
        std::string boolean_lit { "true;" };
        std::string nothing_lit { "nothing;" };
        std::string symbol_lit { "@foo;" };

        BOOST_TEST_MESSAGE("Parsing boolean literal " << boolean_lit);
        string_input<> in1(boolean_lit, "test");

        auto tree = tree_builder<gr::bare_expression>(in1);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(BareExpression,(Boolean,true))"));

        BOOST_TEST_MESSAGE("Parsing nothing literal " << nothing_lit);
        string_input<> in2(nothing_lit, "test");

        tree = tree_builder<gr::bare_expression>(in2);

        node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(BareExpression,(Nothing))"));

        BOOST_TEST_MESSAGE("Parsing symbol literal " << symbol_lit);
        string_input<> in3(symbol_lit, "test");

        tree = tree_builder<gr::bare_expression>(in3);

        node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(BareExpression,(Symbol,foo))"));
    }

    BOOST_AUTO_TEST_CASE (builder_variable_declaration)
    {
        std::string sample { "var x = y * z;" };

        BOOST_TEST_MESSAGE("Parsing variable declaration " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(Variable,(Identifier,x),(BinaryOp,2,(Identifier,y),(Identifier,z)))"));
    }

    BOOST_AUTO_TEST_CASE (builder_constant_declaration)
    {
        std::string sample { "const x = 42;" };

        BOOST_TEST_MESSAGE("Parsing constant declaration " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(Constant,(Identifier,x),(Integral,42,0))"));
    }

    BOOST_AUTO_TEST_CASE (builder_assign_statement)
    {
        std::string sample { "foo = bar ** 2;" };

        BOOST_TEST_MESSAGE("Parsing assignment " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(Assign,(Identifier,foo),(BinaryOp,5,(Identifier,bar),(Integral,2,0)))"));
    }

    BOOST_AUTO_TEST_CASE (builder_compound_assign_statement)
    {
        std::string sample { "i -= 1;" };

        BOOST_TEST_MESSAGE("Parsing compound assignment " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(CompoundAssign,(Identifier,i),(Integral,1,0),1)"));
    }

    BOOST_AUTO_TEST_CASE (builder_do_statement)
    {
        std::string sample { "do foo;" };
        
        BOOST_TEST_MESSAGE("Parsing 'do' statement " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(Do,(Identifier,foo))"));
    }

    BOOST_AUTO_TEST_CASE (builder_statement_block)
    {
        std::string sample { "{do foo; do bar;}" };

        BOOST_TEST_MESSAGE("Parsing statement block " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement_block>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(Block,(Do,(Identifier,foo)),(Do,(Identifier,bar)))"));
    }

    BOOST_AUTO_TEST_SUITE_END ()
}