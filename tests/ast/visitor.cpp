#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/algorithm/string/split.hpp>

#include <string>
#include <vector>
#include <memory>

#include "../../include/ast.hpp"
#include "../../include/visitor/visitor.hpp"
#include "../../include/visitor/default.hpp"
#include "../../include/compat.hpp"

#include "test_setup.hpp"

namespace data = boost::unit_test::data;
namespace ast = rhea::ast;

namespace {
    
    using rhea::util::any;
    using rhea::util::any_cast;
    using rhea::util::bad_any_cast;

    struct StringVisitor : rhea::visitor::DefaultVisitor
    {
        any visit(ast::Boolean* n) { return n->to_string(); }
        any visit(ast::Integer* n) { return n->to_string(); }
        any visit(ast::Double* n) { return n->to_string(); }
    };

    struct VisitorFixture
    {
        VisitorFixture() : v() {}

        StringVisitor v;
    };

    // Datasets

    // Test cases
    BOOST_FIXTURE_TEST_SUITE (AST_visitor, VisitorFixture)

    BOOST_AUTO_TEST_CASE (visit_boolean_ast)
    {
        std::unique_ptr<ast::ASTNode> node = std::make_unique<ast::Boolean>(true);

        auto result = node->visit(&v);

        BOOST_TEST_MESSAGE("Visiting boolean AST node " << node->to_string());
        BOOST_TEST((any_cast<std::string>(result) == "(Boolean,true)"));
    }

    BOOST_AUTO_TEST_CASE (visit_integral_ast)
    {
        std::unique_ptr<ast::ASTNode> node = std::make_unique<ast::Integer>(42);

        auto result = node->visit(&v);

        BOOST_TEST_MESSAGE("Visiting integer AST node " << node->to_string());
        BOOST_TEST((any_cast<std::string>(result) == "(Integral,42,0)"));
    }

    BOOST_AUTO_TEST_CASE (visit_floating_point_ast)
    {
        std::unique_ptr<ast::ASTNode> node = std::make_unique<ast::Double>(1e-6);

        auto result = node->visit(&v);

        BOOST_TEST_MESSAGE("Visiting integer AST node " << node->to_string());
        BOOST_TEST((any_cast<std::string>(result) == "(FloatingPoint,1.0e-06,3)"));
    }

    BOOST_AUTO_TEST_SUITE_END ()
}