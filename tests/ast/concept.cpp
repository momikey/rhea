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
    BOOST_AUTO_TEST_SUITE (AST_Concepts)

    BOOST_AUTO_TEST_CASE (simple_concept_match)
    {
        auto id = std::make_unique<ast::Identifier>("string");
        auto tn = std::make_unique<ast::Typename>(std::move(id));
        std::string name { "foo" };

        auto node = std::make_unique<ast::ConceptMatch>(name, std::move(tn));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());
        BOOST_TEST((node->to_string() == "(ConceptMatch,foo,(Typename,(Identifier,string),null,null))"));
    }

    BOOST_AUTO_TEST_CASE (member_check)
    {
        auto node = std::make_unique<ast::MemberCheck>("foo", "bar");

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());
        BOOST_TEST((node->to_string() == "(MemberCheck,foo,bar)"));
    }

    BOOST_AUTO_TEST_CASE (function_check)
    {
        std::string tn { "Ty" };
        auto tn_id = ast::make_identifier<ast::Identifier>(tn);
        auto tn_node = std::make_unique<ast::Typename>(std::move(tn_id));
        auto fn = ast::make_identifier<ast::Identifier>("f");
        auto rt = ast::make_identifier<ast::Identifier>("string");
        auto rt_node = std::make_unique<ast::Typename>(std::move(rt));

        ast::child_vector<ast::Typename> args;
        args.emplace_back(std::move(tn_node));

        auto node = std::make_unique<ast::FunctionCheck>(tn, std::move(fn), args, std::move(rt_node));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());
        BOOST_TEST((node->to_string() == "(FunctionCheck,Ty,(Identifier,f),(Typename,(Identifier,string),null,null),(Typename,(Identifier,Ty),null,null))"));
    }

    BOOST_AUTO_TEST_SUITE_END ()
}