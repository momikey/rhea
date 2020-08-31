#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <string>
#include <memory>
#include <vector>
#include <iostream>

#include "../../include/codegen/function_visitor.hpp"
#include "../../include/ast.hpp"
#include "../../include/grammar.hpp"
#include "../../include/types/types.hpp"
#include "../../include/debug/parse_tree.hpp"
#include "../../include/debug/build_ast.hpp"

#include <tao/pegtl.hpp>

#include "test_setup.hpp"

namespace data = boost::unit_test::data;
namespace ast = rhea::ast;
namespace cg = rhea::codegen;
namespace util = rhea::util;
namespace types = rhea::types;

namespace {
    // Datasets

    // Test cases

    BOOST_AUTO_TEST_SUITE (Function_visitor)

    BOOST_AUTO_TEST_CASE (Fn_visitor_create)
    {
        BOOST_TEST_MESSAGE("Creating function visitor");

        cg::FunctionVisitor fv;

        auto fv_type = fv.return_type.type();

        BOOST_TEST((util::get_if<types::UnknownType>(&fv_type) != nullptr));
    }

    BOOST_AUTO_TEST_CASE (simple_function_visitor)
    {
        BOOST_TEST_MESSAGE("Testing visitation of simple function");

        std::string fn = "def f = { return 42; }";
        
        auto in = rhea::debug::input_from_string(fn);
        auto parse = rhea::debug::parse<rhea::ast::parser_node>(*in);
        // We take the front child because the debug parser tries to parse a program first.
        auto tree = rhea::debug::build_ast(parse->children.front().get());

        cg::FunctionVisitor fv;
        tree->visit(&fv);

        BOOST_TEST((fv.potential_return_types.size() == 1));
        auto fv_type = fv.potential_return_types.back().type();
        auto as_simple = util::get_if<types::SimpleType>(&fv_type);

        BOOST_TEST((as_simple != nullptr));
        BOOST_TEST((as_simple->type == types::BasicType::Integer));
    }

    BOOST_AUTO_TEST_CASE (branching_function_visitor)
    {
        BOOST_TEST_MESSAGE("Testing visitation of branching function");

        std::string fn = "def f = if (x > 0) { return 42; } else { return 24; }";
        
        auto in = rhea::debug::input_from_string(fn);
        auto parse = rhea::debug::parse<rhea::ast::parser_node>(*in);
        // We take the front child because the debug parser tries to parse a program first.
        auto tree = rhea::debug::build_ast(parse->children.front().get());

        cg::FunctionVisitor fv;
        tree->visit(&fv);

        BOOST_TEST((fv.potential_return_types.size() == 2));
        auto fv_type = fv.potential_return_types.back().type();
        auto as_simple = util::get_if<types::SimpleType>(&fv_type);

        BOOST_TEST((as_simple != nullptr));
        BOOST_TEST((as_simple->type == types::BasicType::Integer));
    }

    BOOST_AUTO_TEST_CASE (void_function_visitor)
    {
        BOOST_TEST_MESSAGE("Testing visitation of a function with no return");

        std::string fn = "def f = { x = 42; }";
        
        auto in = rhea::debug::input_from_string(fn);
        auto parse = rhea::debug::parse<rhea::ast::parser_node>(*in);
        // We take the front child because the debug parser tries to parse a program first.
        auto tree = rhea::debug::build_ast(parse->children.front().get());
        BOOST_TEST_MESSAGE(tree->to_string());

        cg::FunctionVisitor fv;
        tree->visit(&fv);

        BOOST_TEST((fv.potential_return_types.empty()));
    }

    BOOST_AUTO_TEST_SUITE_END ()
}