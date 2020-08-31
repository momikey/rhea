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

    // Test cases
    BOOST_AUTO_TEST_SUITE (AST_modules)

    BOOST_AUTO_TEST_CASE (program_skeleton_ast)
    {
        ast::child_vector<ast::Statement> stmts;
        auto node = std::make_unique<ast::Program>(stmts);

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        BOOST_TEST((node->to_string() == "(Program)"));
    }

    BOOST_AUTO_TEST_CASE (module_skeleton_ast)
    {
        auto n = ast::make_identifier<ast::Identifier>("mymodule");
        auto modname = std::make_unique<ast::ModuleName>(std::move(n));
        auto moddef = ast::make_statement<ast::ModuleDef>(std::move(modname));

        ast::child_vector<ast::Statement> stmts;
        stmts.emplace_back(std::move(moddef));

        auto node = std::make_unique<ast::Module>(stmts);

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());

        BOOST_TEST((node->to_string() == "(Module,(ModuleDef,(ModuleName,mymodule)))"));
    }

    BOOST_AUTO_TEST_CASE (use_statement_ast)
    {
        std::unique_ptr<ast::AnyIdentifier> m = ast::make_identifier<ast::Identifier>("foo");
        auto name = std::make_unique<ast::ModuleName>(std::move(m));

        auto node = std::make_unique<ast::Use>(std::move(name));

        BOOST_TEST_MESSAGE("Testing AST Node" << node->to_string());

        BOOST_TEST((node->to_string()) == "(Use,(ModuleName,foo))");
    }
    
    BOOST_AUTO_TEST_CASE (use_statement_fqn_ast)
    {
        std::vector<std::string> names { "foo", "bar", "baz" };
        auto m = ast::make_identifier<ast::FullyQualified>(names);
        auto name = std::make_unique<ast::ModuleName>(std::move(m));

        auto node = std::make_unique<ast::Use>(std::move(name));

        BOOST_TEST_MESSAGE("Testing AST Node" << node->to_string());

        BOOST_TEST((node->to_string()) == "(Use,(ModuleName,foo:bar:baz))");
    }

    BOOST_AUTO_TEST_CASE (module_statement_ast)
    {
        std::unique_ptr<ast::AnyIdentifier> m = ast::make_identifier<ast::Identifier>("foo");
        auto name = std::make_unique<ast::ModuleName>(std::move(m));

        auto node = std::make_unique<ast::ModuleDef>(std::move(name));

        BOOST_TEST_MESSAGE("Testing AST Node" << node->to_string());

        BOOST_TEST((node->to_string()) == "(ModuleDef,(ModuleName,foo))");
    }
    
    BOOST_AUTO_TEST_CASE (module_statement_fqn_ast)
    {
        std::vector<std::string> names { "foo", "bar", "baz" };
        auto m = ast::make_identifier<ast::FullyQualified>(names);
        auto name = std::make_unique<ast::ModuleName>(std::move(m));

        auto node = std::make_unique<ast::ModuleDef>(std::move(name));

        BOOST_TEST_MESSAGE("Testing AST Node" << node->to_string());

        BOOST_TEST((node->to_string()) == "(ModuleDef,(ModuleName,foo:bar:baz))");
    }
    
    BOOST_AUTO_TEST_CASE (import_statement_ast)
    {
        std::unique_ptr<ast::AnyIdentifier> m = ast::make_identifier<ast::Identifier>("mymodule");
        auto name = std::make_unique<ast::ModuleName>(std::move(m));

        ast::child_vector<ast::Identifier> ids;
        ids.emplace_back(std::make_unique<ast::Identifier>("Foo"));
        ids.emplace_back(std::make_unique<ast::Identifier>("Bar"));

        auto node = std::make_unique<ast::Import>(ids, std::move(name));

        BOOST_TEST_MESSAGE("Testing AST Node" << node->to_string());

        BOOST_TEST((node->to_string()) == "(Import,(ModuleName,mymodule),(Identifier,Foo),(Identifier,Bar))");
    }
    
    BOOST_AUTO_TEST_CASE (export_statement_ast)
    {
        ast::child_vector<ast::Identifier> ids;
        ids.emplace_back(std::make_unique<ast::Identifier>("Foo"));
        ids.emplace_back(std::make_unique<ast::Identifier>("Bar"));

        auto node = std::make_unique<ast::Export>(ids);

        BOOST_TEST_MESSAGE("Testing AST Node" << node->to_string());

        BOOST_TEST((node->to_string()) == "(Export,(Identifier,Foo),(Identifier,Bar))");
    }

    BOOST_AUTO_TEST_SUITE_END ()
}