#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <string>

#include <tao/pegtl.hpp>
#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/tracer.hpp>

#include "test_setup.hpp"

#include "../../include/grammar/statement.hpp"

namespace data = boost::unit_test::data;

using tao::pegtl::parse;
using tao::pegtl::string_input;
namespace rg = rhea::grammar;

////////////////////
//// Sample data
////////////////////

std::string simple_assign_samples[] = {
    "x = 1",
    "y = 42",
    "a_long_name = 3.14 * a**2 + another_number"
};

std::string compound_assign_samples[] = {
    "i += 1",
    "abc *= def",
    "n <<= x % 8",
    "BITMASK |= 0xf0"
};

std::string constant_decl_samples[] = {
    "const C = 69",
    "const MY_CONSTANT = C ** 3",
    "const ABC = (abc == true)"
};

std::string variable_decl_samples[] = {
    "var n as integer",
    "var n = 6",
    "var long_variable_name = a**2 + b**2",
    "var is_working = false"
};

std::string if_statement_samples[] = {
    "if (x < 10) { result = print(x); }",
    "if my_function() { result = do_this(); } else { result = do_that(); }"
};

std::string unless_statement_samples[] = {
    "unless (x < 10) { print('test'); }"
};

std::string while_statement_samples[] = {
    "while (not s) { s.doIt(); }"
};

std::string for_statement_samples[] = {
    "for i in range(10) {}",
    "for a in b print(a);"
};

std::string do_statement_samples[] = {
    "do some_function()",
    "do (f(), g(), h())",
    "do some_function # without parens"
};

std::string with_statement_samples[] = {
    "with (s as string) {}",
    "with n as integer n = 42;"
};

std::string on_case_samples[] = {
    "on 1: print('test');",
    "on 2: { do_something(); }",
    "on @foo: { bar = 42_u; }"
};

std::string when_case_samples[] = {
    "when nonzero?: { print('Not zero'); }",
    "when zero?: print('Zero');"
};

std::string default_case_samples[] = {
    "default: doIt();",
    "default: { if x == 42 print(the_answer); else quit(); }"
};

std::string match_on_samples[] = {
    "match foo {\n"
    "   on 1: print('test');\n"
    "   on 2: { do_something_else(foo); }\n"
    "   default: { result = error; }\n"
    "}"
};

std::string match_when_samples[] = {
    "match foo {\n"
    "   when nonzero?: { print('Not zero'); }\n"
    "   when zero?: { print('Zero'); }\n"
    "}"
};

std::string enum_declaration_samples[] = {
    "type En = @{foo, bar, baz}"
};

std::string structure_declaration_samples[] = {
    "type Person = {\n"
    "   name: string,\n"
    "   age: integer\n"
    "}"
};

std::string type_alias_samples[] = {
    "type my_int = integer",
    "type StringList = list <string>",
    "type File = std:fs:File"
};

std::string match_type_samples[] = {
    "match arg {"
    "   type integer: print('Integer');"
    "   type string: print('String');"
    "}"
};

std::string throw_samples[] = {
    "throw foo",
    "throw error('Something went wrong')"
};

std::string try_statement_samples[] = {
    "try {\n"
    "   z = x / y;\n"
    "} catch { e: divide_by_zero }\n"
    "{\n"
    "   print(e.message);\n"
    "} finally {\n"
    "   print('finally');\n"
    "}"
};

std::string arguments_list_samples[] = {
    "{ n: integer }",
    "{ xyz: string, abc: list<integer> }",
    "{ foo: Enum, bar: A_Structure, baz: |a, b, c| }"
};

std::string fn_predicate_call_samples[] = {
    "foo.bar?",
    "x.y(z)?",
    "arg.my_predicate(1234, 'abc')?"
};

std::string with_block_samples[] = {
    "with {\n"
    "   y.not_negative?,\n"
    "   y.less_than(32)?\n"
    "}"
};

std::string unchecked_function_samples[] = {
    "def f! = print('test');",
    "def unchecked! <Ty ~> Stringable> [string] { s: Ty } = {\n"
    "   print(s);\n"
    "}"
};

std::string predicate_function_samples[] = {
    "def f? { n: integer } = return n < 0;",
    "def predicate? <Ty ~> Numeric> { n : Ty }\n"
    "with { n.not_equal(0 as Ty)? } = {\n"
    "   return ^1 / n;\n"
    "}"
};

std::string operator_function_samples[] = {
    "def add$ { l: Complex, r: Complex } =\n"
    "{\n"
    "   return Complex(l.real + r.real, l.imag + r.imag);\n"
    "}"
};

std::string basic_function_samples[] = {
    "def my_function = return 'Hello, world!';",

    "def power [integer] { x : integer, y : integer }\n"
    "with { y.not_negative?, y.less_than(32)? } =\n"
    "{ return x ** y; }"
};

////////////////////
//// Test cases
////////////////////

BOOST_AUTO_TEST_SUITE (Analysis)

BOOST_AUTO_TEST_CASE(no_infinite_recursion)
{
    auto issues { tao::pegtl::analyze<
        simple_parser<rg::statement>
    >() };

    BOOST_TEST(issues == 0u);
}

BOOST_AUTO_TEST_SUITE_END ()

BOOST_AUTO_TEST_SUITE (Assignments)

BOOST_DATA_TEST_CASE(simple_assignment, data::make(simple_assign_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::assignment>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(compound_assignment, data::make(compound_assign_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::compound_assignment>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(constant_declaration, data::make(constant_decl_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::constant_declaration>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(variable_declaration, data::make(variable_decl_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::variable_declaration>
    >(in) == true);
}

BOOST_AUTO_TEST_SUITE_END ()

BOOST_AUTO_TEST_SUITE (Statements)

BOOST_AUTO_TEST_CASE(statement_block)
{
    std::string valid {
        "{ var x = 42;\n\tx *= 3;\n\tvar y = x/2; }"
    };

    BOOST_TEST_MESSAGE("Parsing " << valid);
    string_input<> in(valid, "test");
    BOOST_TEST(parse<
        simple_parser<rg::statement_block>
    >(in) == true); 
}

BOOST_DATA_TEST_CASE(if_statement, data::make(if_statement_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::if_statement>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(unless_statement, data::make(unless_statement_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::unless_statement>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(while_statement, data::make(while_statement_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::while_statement>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(for_statement, data::make(for_statement_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::for_statement>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(do_statement, data::make(do_statement_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::do_statement>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(with_statement, data::make(with_statement_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::with_statement>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(on_case, data::make(on_case_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::on_case>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(when_case, data::make(when_case_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::when_case>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(default_case, data::make(default_case_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::default_case>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(match_on_statement, data::make(match_on_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::match_on_statement>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(match_when_statement, data::make(match_when_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::match_when_statement>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(match_type_statement, data::make(match_type_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::match_type_statement>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(enum_declaration, data::make(enum_declaration_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::enum_declaration>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(structure_declaration, data::make(structure_declaration_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::structure_declaration>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(type_alias, data::make(type_alias_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::type_alias>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(throw_statement, data::make(throw_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::throw_statement>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(try_statement, data::make(try_statement_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::try_statement>
    >(in) == true);
}

BOOST_AUTO_TEST_SUITE_END ()

BOOST_AUTO_TEST_SUITE (Functions)

BOOST_DATA_TEST_CASE(arguments_list, data::make(arguments_list_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::arguments_list>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(fn_predicate_call, data::make(fn_predicate_call_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::fn_condition_predicate_call>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(with_block, data::make(with_block_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::fn_with_block>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(unchecked_function_def, data::make(unchecked_function_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::unchecked_function_def>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(predicate_function_def, data::make(predicate_function_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::predicate_function_def>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(operator_function_def, data::make(operator_function_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::operator_function_def>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(basic_function_def, data::make(basic_function_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::basic_function_def>
    >(in) == true);
}

BOOST_AUTO_TEST_SUITE_END ()
