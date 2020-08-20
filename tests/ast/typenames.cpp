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

        auto node = std::make_unique<ast::Typename>(std::move(name));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());
        BOOST_TEST((node->name->to_string() == "(Identifier,Foo)"));
        BOOST_TEST((node->generic_part == nullptr));
        BOOST_TEST((node->array_part == nullptr));

        BOOST_TEST_MESSAGE("Testing canonical representation " << node->canonical_name());
        BOOST_TEST((node->canonical_name() == "Foo"));
    }

    BOOST_AUTO_TEST_CASE (generic_typenames)
    {
        auto name = ast::make_identifier<ast::Identifier>("Foo");
        auto gname = ast::make_identifier<ast::Identifier>("Ty");
        ast::child_vector<ast::Typename> gens;
        gens.emplace_back(std::make_unique<ast::Typename>(std::move(gname)));

        auto gnode = std::make_unique<ast::GenericTypename>(gens);
        auto node = std::make_unique<ast::Typename>(std::move(name), std::move(gnode));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());
        BOOST_TEST((node->name->to_string() == "(Identifier,Foo)"));
        BOOST_TEST((node->generic_part->to_string() == "(GenericTypename,(Typename,(Identifier,Ty),null,null))"));
        BOOST_TEST((node->array_part == nullptr));

        BOOST_TEST_MESSAGE("Testing canonical representation " << node->canonical_name());
        BOOST_TEST((node->canonical_name() == "Foo<Ty>"));
    }

    BOOST_AUTO_TEST_CASE (array_typenames)
    {
        auto name = ast::make_identifier<ast::Identifier>("Foo");
        auto array = ast::make_expression<ast::Integer>(42);
        ast::child_vector<ast::Expression> ts;
        ts.emplace_back(std::move(array));
        auto array_typename = std::make_unique<ast::ArrayTypename>(ts);

        auto node = std::make_unique<ast::Typename>(std::move(name), std::move(array_typename));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());
        BOOST_TEST((node->name->to_string() == "(Identifier,Foo)"));
        BOOST_TEST((node->generic_part == nullptr));
        BOOST_TEST((node->array_part->to_string() == "(ArrayTypename,(Integral,42,0))"));
    }

    BOOST_AUTO_TEST_CASE (optional_typename)
    {
        auto id = ast::make_identifier<ast::Identifier>("Foo");
        auto tn = std::make_unique<ast::Typename>(std::move(id));

        auto node = std::make_unique<ast::Optional>(std::move(tn));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());
        BOOST_TEST((node->to_string() == "(Optional,(Typename,(Identifier,Foo),null,null))"));

        BOOST_TEST_MESSAGE("Testing canonical representation " << node->canonical_name());
        BOOST_TEST((node->canonical_name() == "Optional(Foo)"));
    }

    BOOST_AUTO_TEST_CASE (variant_typename)
    {
        ast::child_vector<ast::AnyIdentifier> ids;
        ids.emplace_back(std::make_unique<ast::Identifier>("Foo"));
        ids.emplace_back(std::make_unique<ast::Identifier>("Bar"));

        ast::child_vector<ast::Typename> ts;
        for (auto& id : ids)
        {
            ts.emplace_back(std::make_unique<ast::Typename>(std::move(id)));
        }

        auto node = std::make_unique<ast::Variant>(ts);

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());
        BOOST_TEST((node->to_string() == "(Variant,(Typename,(Identifier,Foo),null,null),(Typename,(Identifier,Bar),null,null))"));

        BOOST_TEST_MESSAGE("Testing canonical representation " << node->canonical_name());
        BOOST_TEST((node->canonical_name() == "Variant(Foo,Bar)"));
    }

    BOOST_AUTO_TEST_CASE (cast_operator)
    {
        auto lhs = ast::make_expression<ast::Integer>(42);
        auto rid = ast::make_identifier<ast::Identifier>("ubyte");
        auto rhs = std::make_unique<ast::Typename>(std::move(rid));

        auto node = std::make_unique<ast::Cast>(std::move(lhs), std::move(rhs));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());
        BOOST_TEST((node->to_string() == "(Cast,(Integral,42,0),(Typename,(Identifier,ubyte),null,null))"));
    }

    BOOST_AUTO_TEST_CASE (check_operator)
    {
        auto lhs = ast::make_expression<ast::Integer>(42);
        auto rid = ast::make_identifier<ast::Identifier>("integer");
        auto rhs = std::make_unique<ast::Typename>(std::move(rid));

        auto node = std::make_unique<ast::TypeCheck>(std::move(lhs), std::move(rhs));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());
        BOOST_TEST((node->to_string() == "(TypeCheck,(Integral,42,0),(Typename,(Identifier,integer),null,null))"));
    }

    BOOST_AUTO_TEST_CASE (alias_statement)
    {
        auto a = std::make_unique<ast::Identifier>("Foo");
        auto o = ast::make_identifier<ast::Identifier>("Bar");

        auto node = std::make_unique<ast::Alias>(std::move(a), std::move(o));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());
        BOOST_TEST((node->to_string() == "(Alias,(Identifier,Foo),(Identifier,Bar))"));
    }

    BOOST_AUTO_TEST_CASE (symbol_list_ast)
    {
        std::vector<std::string> ss = { "foo", "bar", "baz" };

        auto node = std::make_unique<ast::SymbolList>(ss);

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());
        BOOST_TEST((node->to_string() == "(SymbolList,(Symbol,foo),(Symbol,bar),(Symbol,baz))"));
    }

    BOOST_AUTO_TEST_CASE (enum_declaration_ast)
    {
        std::vector<std::string> ss = { "foo", "bar", "baz" };
        auto sl = std::make_unique<ast::SymbolList>(ss);
        auto id = std::make_unique<ast::Identifier>("En");

        auto node = std::make_unique<ast::Enum>(std::move(id), std::move(sl));

        BOOST_TEST_MESSAGE("Testing AST Node " << node->to_string());
        BOOST_TEST((node->to_string() ==
            "(Enum,(Identifier,En),(SymbolList,(Symbol,foo),(Symbol,bar),(Symbol,baz)))"));
    }
   
    BOOST_AUTO_TEST_SUITE_END ()
}