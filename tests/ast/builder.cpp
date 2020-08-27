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

    BOOST_AUTO_TEST_CASE (builder_fully_qualified_expression)
    {
        std::string sample { "foo:bar:baz;" };

        BOOST_TEST_MESSAGE("Parsing fully-qualified identifier expression " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::bare_expression>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() == "(BareExpression,(FullyQualified,foo,bar,baz))"));
    }

    BOOST_AUTO_TEST_CASE (builder_relative_identifier_expression)
    {
        std::string sample { ":foo:bar;" };

        BOOST_TEST_MESSAGE("Parsing relative identifier expression " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::bare_expression>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() == "(BareExpression,(RelativeIdentifier,foo,bar))"));
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
        std::string short_hex_lit { "0x1234;" };
        std::string long_hex_lit { "0x0000111122223333;" };
        std::string string_lit { "\"abc\";" };

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

        BOOST_TEST_MESSAGE("Parsing short hex literal " << short_hex_lit);
        string_input<> in4(short_hex_lit, "test");

        tree = tree_builder<gr::bare_expression>(in4);

        node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(BareExpression,(Integral,4660,5))"));

        BOOST_TEST_MESSAGE("Parsing long hex literal " << long_hex_lit);
        string_input<> in5(long_hex_lit, "test");

        tree = tree_builder<gr::bare_expression>(in5);

        node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() == "(BareExpression,(Integral,18765284782899,7))"));

        BOOST_TEST_MESSAGE("Parsing string literal " << string_lit);
        string_input<> in6(string_lit, "test");

        tree = tree_builder<gr::bare_expression>(in6);

        node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() == "(BareExpression,(String,\"abc\"))"));
    }

    BOOST_AUTO_TEST_CASE (builder_vector_expressions)
    {
        std::string array_expr { "[1,2,3];" };
        std::string list_expr { "(1,2,3);" };
        std::string tuple_expr { "({1,2,3});" };
        std::string symlist_expr { "@{a,b,c};" };
        std::string dictionary_expr { "{@a: 1, @b: 2};" };

        BOOST_TEST_MESSAGE("Parsing array expression " << array_expr);
        string_input<> in1(array_expr, "test");

        auto tree = tree_builder<gr::bare_expression>(in1);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(BareExpression,(Array,(Integral,1,0),(Integral,2,0),(Integral,3,0)))"));

        BOOST_TEST_MESSAGE("Parsing list expression " << list_expr);
        string_input<> in2(list_expr, "test");

        tree = tree_builder<gr::bare_expression>(in2);

        node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(BareExpression,(List,(Integral,1,0),(Integral,2,0),(Integral,3,0)))"));

        BOOST_TEST_MESSAGE("Parsing tuple expression " << tuple_expr);
        string_input<> in3(tuple_expr, "test");

        tree = tree_builder<gr::bare_expression>(in3);

        node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(BareExpression,(Tuple,(Integral,1,0),(Integral,2,0),(Integral,3,0)))"));

        BOOST_TEST_MESSAGE("Parsing symbol list expression " << symlist_expr);
        string_input<> in4(symlist_expr, "test");

        tree = tree_builder<gr::bare_expression>(in4);

        node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(BareExpression,(SymbolList,(Symbol,a),(Symbol,b),(Symbol,c)))"));

        BOOST_TEST_MESSAGE("Parsing dictionary expression " << dictionary_expr);
        string_input<> in5(dictionary_expr, "test");

        tree = tree_builder<gr::bare_expression>(in5);

        node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(BareExpression,(Dictionary,(DictionaryEntry,(Symbol,a),(Integral,1,0)),(DictionaryEntry,(Symbol,b),(Integral,2,0))))"));
    }

    BOOST_AUTO_TEST_CASE (builder_member_expression)
    {
        std::string sample { "x.y;" };

        BOOST_TEST_MESSAGE("Parsing member expression " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::bare_expression>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(BareExpression,(Member,(Identifier,y),(Identifier,x)))"));
    }

    BOOST_AUTO_TEST_CASE (builder_subscript_expression)
    {
        std::string sample { "x[y];" };

        BOOST_TEST_MESSAGE("Parsing subscript operation " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::bare_expression>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(BareExpression,(Subscript,(Identifier,x),(Identifier,y)))"));
    }

    BOOST_AUTO_TEST_CASE (builder_ternary_expression)
    {
        std::string sample { "(if a then b else c);" };

        BOOST_TEST_MESSAGE("Parsing ternary operation " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::bare_expression>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(BareExpression,(TernaryOp,(Identifier,a),(Identifier,b),(Identifier,c)))"));
    }

    BOOST_AUTO_TEST_CASE (builder_cast_expression)
    {
        std::string sample { "x as long;" };

        BOOST_TEST_MESSAGE("Parsing cast operation " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::bare_expression>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(BareExpression,(Cast,(Identifier,x),(Typename,(Identifier,long),null,null)))"));
    }

    BOOST_AUTO_TEST_CASE (builder_typecheck_expression)
    {
        std::string sample { "x is long;" };

        BOOST_TEST_MESSAGE("Parsing type-check operation " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::bare_expression>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(BareExpression,(TypeCheck,(Identifier,x),(Typename,(Identifier,long),null,null)))"));
    }

    BOOST_AUTO_TEST_CASE (builder_function_call_expressions)
    {
        std::string empty_args { "f();" };
        std::string pos_args { "f(1,2);" };
        std::string named_args { "f(a: 1, b: 2);" };

        BOOST_TEST_MESSAGE("Parsing function call with empty arguments " << empty_args);
        string_input<> in_empty(empty_args, "test");

        auto tree = tree_builder<gr::bare_expression>(in_empty);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(BareExpression,(Call,(Identifier,f)))"));

        BOOST_TEST_MESSAGE("Parsing function call with positional arguments " << pos_args);
        string_input<> in_pos(pos_args, "test");

        tree = tree_builder<gr::bare_expression>(in_pos);

        node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(BareExpression,(Call,(Identifier,f),(Integral,1,0),(Integral,2,0)))"));

        BOOST_TEST_MESSAGE("Parsing function call with named arguments " << named_args);
        string_input<> in_named(named_args, "test");

        tree = tree_builder<gr::bare_expression>(in_named);

        node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() ==
            "(BareExpression,(Call,(Identifier,f),(NamedArgument,a,(Integral,1,0)),(NamedArgument,b,(Integral,2,0))))"));
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

    BOOST_AUTO_TEST_CASE (builder_enum_declaration)
    {
        std::string sample { "type En = @{a,b,c};" };
        
        BOOST_TEST_MESSAGE("Parsing enum declaration " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(Enum,(Identifier,En),(SymbolList,(Symbol,a),(Symbol,b),(Symbol,c)))"));
    }

    BOOST_AUTO_TEST_CASE (builder_structure_declaration)
    {
        std::string sample { "type Person = { name: string, age: integer };" };
        
        BOOST_TEST_MESSAGE("Parsing structure declaration " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(Structure,(Identifier,Person),(TypePair,name,(Typename,(Identifier,string),null,null)),(TypePair,age,(Typename,(Identifier,integer),null,null)))"));
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

    BOOST_AUTO_TEST_CASE (builder_if_statement)
    {
        std::string no_else { "if x do foo;" };
        
        BOOST_TEST_MESSAGE("Parsing 'if' statement " << no_else);
        string_input<> in(no_else, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(If,(Identifier,x),(Do,(Identifier,foo)),null)"));

        std::string with_else { "if y do foo; else do bar;"};

        BOOST_TEST_MESSAGE("Parsing 'if-else' statement " << with_else);
        string_input<> in2(with_else, "test");

        tree = tree_builder<gr::statement>(in2);

        node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(If,(Identifier,y),(Do,(Identifier,foo)),(Do,(Identifier,bar)))"));
    }

    BOOST_AUTO_TEST_CASE (builder_unless_statement)
    {
        std::string sample { "unless foo do foo;" };
        
        BOOST_TEST_MESSAGE("Parsing 'unless' statement " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(If,(Identifier,foo),null,(Do,(Identifier,foo)))"));
    }

    BOOST_AUTO_TEST_CASE (builder_while_statement)
    {
        std::string sample { "while (x < 10) { x += 1; }" };
        
        BOOST_TEST_MESSAGE("Parsing 'while' statement " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() ==
            "(While,(BinaryOp,10,(Identifier,x),(Integral,10,0)),(Block,(CompoundAssign,(Identifier,x),(Integral,1,0),0)))"));
    }

    BOOST_AUTO_TEST_CASE (builder_for_statement)
    {
        std::string sample { "for i in range { do foo; }" };
        
        BOOST_TEST_MESSAGE("Parsing 'for' statement " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(For,i,(Identifier,range),(Block,(Do,(Identifier,foo))))"));
    }

    BOOST_AUTO_TEST_CASE (builder_break_continue_statements)
    {
        std::string sample { "{ break; continue; }" };
        
        BOOST_TEST_MESSAGE("Parsing 'break' and 'continue' statements " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement_block>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(Block,(Break),(Continue))"));
    }

    BOOST_AUTO_TEST_CASE (builder_return_statement)
    {
        std::string sample { "return false;" };
        
        BOOST_TEST_MESSAGE("Parsing return statement " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(Return,(Boolean,false))"));
    }

    BOOST_AUTO_TEST_CASE (builder_extern_declaration)
    {
        std::string sample { "extern foo;" };

        BOOST_TEST_MESSAGE("Parsing extern declaration " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(Extern,foo)"));
    }

    BOOST_AUTO_TEST_CASE (builder_typenames)
    {
        std::string simple { "var x as y;" };
        std::string fq_type { "var x as foo:bar:baz;" };
        std::string relative_type { "var x as :foo:bar;" };
        std::string generic_type { "var x as list <string>;" };
        std::string array_type { "var x as byte[16];" };
        std::string array_generic_type { "var x as list <string> [10];" };
        std::string multid_array_type { "var x as byte [256][16];" };

        BOOST_TEST_MESSAGE("Parsing simple typename declaration " << simple);
        string_input<> in_simple(simple, "test");

        auto tree = tree_builder<gr::statement>(in_simple);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(TypeDeclaration,(Identifier,x),(Typename,(Identifier,y),null,null))"));

        BOOST_TEST_MESSAGE("Parsing fully-qualified typename declaration " << fq_type);
        string_input<> in_fq(fq_type, "test");

        tree = tree_builder<gr::statement>(in_fq);

        node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() == "(TypeDeclaration,(Identifier,x),(Typename,(FullyQualified,foo,bar,baz),null,null))"));

        BOOST_TEST_MESSAGE("Parsing relative typename declaration " << relative_type);
        string_input<> in_relative(relative_type, "test");

        tree = tree_builder<gr::statement>(in_relative);

        node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() == "(TypeDeclaration,(Identifier,x),(Typename,(RelativeIdentifier,foo,bar),null,null))"));

        BOOST_TEST_MESSAGE("Parsing generic typename declaration " << generic_type);
        string_input<> in_generic(generic_type, "test");

        tree = tree_builder<gr::statement>(in_generic);

        node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() == "(TypeDeclaration,(Identifier,x),(Typename,(Identifier,list),(GenericTypename,(Typename,(Identifier,string),null,null)),null))"));

        BOOST_TEST_MESSAGE("Parsing array typename declaration " << array_type);
        string_input<> in_array(array_type, "test");

        tree = tree_builder<gr::statement>(in_array);

        node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() == "(TypeDeclaration,(Identifier,x),(Typename,(Identifier,byte),null,(ArrayTypename,(Integral,16,0))))"));

        BOOST_TEST_MESSAGE("Parsing array and generic typename declaration " << array_generic_type);
        string_input<> in_array_generic(array_generic_type, "test");

        tree = tree_builder<gr::statement>(in_array_generic);

        node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() == "(TypeDeclaration,(Identifier,x),(Typename,(Identifier,list),(GenericTypename,(Typename,(Identifier,string),null,null)),(ArrayTypename,(Integral,10,0))))"));

        BOOST_TEST_MESSAGE("Parsing multidimensional array typename declaration " << multid_array_type);
        string_input<> in_mdarray(multid_array_type, "test");

        tree = tree_builder<gr::statement>(in_mdarray);

        node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() == "(TypeDeclaration,(Identifier,x),(Typename,(Identifier,byte),null,(ArrayTypename,(Integral,256,0),(Integral,16,0))))"));

    }

    BOOST_AUTO_TEST_CASE (builder_simple_basic_function_def)
    {
        std::string sample { "def f = { return true; }" };

        BOOST_TEST_MESSAGE("Parsing simple function definition " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() == "(Def,0,f,null,null,(Conditions),(Block,(Return,(Boolean,true))))"));
    }

    BOOST_AUTO_TEST_CASE (builder_basic_function_def_return_type)
    {
        std::string sample { "def f [boolean] = { return true; }" };

        BOOST_TEST_MESSAGE("Parsing function definition with return type " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST((node->to_string() ==
            "(Def,0,f,(Typename,(Identifier,boolean),null,null),null,(Conditions),(Block,(Return,(Boolean,true))))"));
    }

    BOOST_AUTO_TEST_CASE (builder_basic_function_def_with_args)
    {
        std::string sample { "def f { t: T } = { return true; }" };

        BOOST_TEST_MESSAGE("Parsing function definition with arguments " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() ==
            "(Def,0,f,null,(Arguments,(TypePair,t,(Typename,(Identifier,T),null,null))),(Conditions),(Block,(Return,(Boolean,true))))"));
    }

    BOOST_AUTO_TEST_CASE (builder_basic_function_def_with_condition)
    {
        std::string sample { "def f { t: T } with { t.p? } = { return true; }" };

        BOOST_TEST_MESSAGE("Parsing function definition with conditions " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() ==
            "(Def,0,f,null,(Arguments,(TypePair,t,(Typename,(Identifier,T),null,null))),(Conditions,(Condition,t,(PredicateCall,(Identifier,p)))),(Block,(Return,(Boolean,true))))"));
    }

    BOOST_AUTO_TEST_CASE (builder_basic_function_def_with_condition_arguments)
    {
        std::string sample { "def f { t: T } with { t.p(42)? } = { return true; }" };

        BOOST_TEST_MESSAGE("Parsing function definition with a condition taking arguments " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() ==
            "(Def,0,f,null,(Arguments,(TypePair,t,(Typename,(Identifier,T),null,null))),(Conditions,(Condition,t,(PredicateCall,(Identifier,p),(Integral,42,0)))),(Block,(Return,(Boolean,true))))"));
    }

    BOOST_AUTO_TEST_CASE (builder_basic_function_def_with_wildcard_arg)
    {
        std::string sample { "def f { t: * } = { return true; }" };

        BOOST_TEST_MESSAGE("Parsing function definition with wildcard argument " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() ==
            "(Def,0,f,null,(Arguments,(TypePair,t,(Typename,(Identifier,$$wildcard$$),null,null))),(Conditions),(Block,(Return,(Boolean,true))))"));
    }

    BOOST_AUTO_TEST_CASE (builder_use_statement)
    {
        std::string sample { "use foo;" };

        BOOST_TEST_MESSAGE("Parsing simple use declaration " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() == "(Use,(ModuleName,foo))"));
    }

    BOOST_AUTO_TEST_CASE (builder_use_statement_relative)
    {
        std::string sample { "use :relative_id;" };

        BOOST_TEST_MESSAGE("Parsing relative use declaration " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() == "(Use,(ModuleName,:relative_id))"));
    }

    BOOST_AUTO_TEST_CASE (builder_use_statement_fully_qualified)
    {
        std::string sample { "use foo:bar:baz;" };

        BOOST_TEST_MESSAGE("Parsing fully-qualified use declaration " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() == "(Use,(ModuleName,foo:bar:baz))"));
    }

    BOOST_AUTO_TEST_CASE (builder_module_declaration)
    {
        std::string sample { "module my_module;" };

        BOOST_TEST_MESSAGE("Parsing simple module declaration " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() == "(ModuleDef,(ModuleName,my_module))"));
    }

    BOOST_AUTO_TEST_CASE (builder_module_declaration_fully_qualified)
    {
        std::string sample { "module org:example:my_module;" };

        BOOST_TEST_MESSAGE("Parsing fully-qualified module declaration " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() == "(ModuleDef,(ModuleName,org:example:my_module))"));
    }

    BOOST_AUTO_TEST_CASE (builder_import_declaration)
    {
        std::string sample { "import { foo, bar } from my_module;" };

        BOOST_TEST_MESSAGE("Parsing import declaration " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() == "(Import,(ModuleName,my_module),(Identifier,foo),(Identifier,bar))"));
    }

    BOOST_AUTO_TEST_CASE (builder_import_declaration_fully_qualified)
    {
        std::string sample { "import { foo, bar } from org:example:my_module;" };

        BOOST_TEST_MESSAGE("Parsing import declaration with fully-qualified module " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() == "(Import,(ModuleName,org:example:my_module),(Identifier,foo),(Identifier,bar))"));
    }

    BOOST_AUTO_TEST_CASE (builder_export_declaration)
    {
        std::string sample { "export { foo, bar };" };

        BOOST_TEST_MESSAGE("Parsing export declaration " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::statement>(in);

        auto node = ast::internal::create_statement_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() == "(Export,(Identifier,foo),(Identifier,bar))"));
    }

    BOOST_AUTO_TEST_CASE (builder_simple_program_definition)
    {
        std::string sample { "def main = { return true; }" };

        BOOST_TEST_MESSAGE("Parsing program definition " << sample);
        string_input<> in(sample, "test");

        auto tree = tree_builder<gr::program_definition>(in);

        auto node = ast::internal::create_top_level_node(tree->children.front().get());

        BOOST_TEST_MESSAGE((node->position));
        BOOST_TEST_MESSAGE((node->to_string()));
        BOOST_TEST((node->to_string() ==
            "(Program,(Def,0,main,null,null,(Conditions),(Block,(Return,(Boolean,true)))))"));
    }

    BOOST_AUTO_TEST_SUITE_END ()
}