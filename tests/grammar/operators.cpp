#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <string>

#include <tao/pegtl.hpp>
#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/tracer.hpp>

#include "test_setup.hpp"

#include "../../include/grammar/operators.hpp"

namespace data = boost::unit_test::data;

using tao::pegtl::parse;
using tao::pegtl::string_input;
namespace rg = rhea::grammar;

// Samples
std::string relation_samples[] = {
    "1 < 2",
    "a > b",
    "n_1 >= 42",
    "ABC != -999",
    "0 == 0"
};

std::string shift_samples[] = {
    "1 << 8",
    "n << n",
    "a >> b + c"
};

std::string addsub_samples[] = {
    "2 + 2",
    "5 --7",
    "42 + a - 0x3"
};

std::string muldiv_samples[] = {
    "2 * 2",
    "32 / 4",
    "-12345 * -1234",
    "n % 123",
    "x / y * z % w"
};

std::string exp_samples[] = {
    "2 ** 3",
    "-2 ** -3",
    "a ** b ** c"
};

std::string unary_samples[] = {
    "~123",
    "-abc",
    "^7",
    "not asdf",
    "not not x"
};

std::string cast_samples[] = {
    "a as integer",
    "123 as byte",
    "0xff as long"
};

std::string array_samples[] = {
    "[1, 2, 3]",
    "[a, b, c]",
    "[4 + 2, 6 * 9]"
};

std::string list_samples[] = {
    "(1, 2, 3)",
    "(a, b, c)",
    "(4 + 2, 6 * 9)",
    "()"
};

std::string tuple_samples[] = {
    "{1, 2, 3}",
    "{a, b, c}",
    "{[1,2,3], ((4.0 / 20.0) + (6 * 9))}",
    "{}"
};

std::string symbol_list_samples[] = {
    "@{foo, bar, baz}",
    "@{A, B, C}"
};

std::string subscript_samples[] = {
    "a[1]",
    "x[y]",
    "n[i+3]",
    "(a[b])[c]",
    "a[b][c]"
};

std::string kv_pair_samples[] = {
    "n: 10",
    "i: j*2",
    "abc: (def**2)+ -10 or false",
    "long_argument: if x > 0 then true else false",
    "Printable : x as string"
};

std::string dictionary_samples[] = {
    "{ @foo: 42, @bar: 's' }"
};

std::string member_access_samples[] = {
    "a.b",
    "x.y.z",
    "array[i].foo"
};

std::string function_call_samples[] = {
    "f()",
    "foo(42)",
    "my_function(x, y, z*2)",
    "a.method_like().call()",
    "predicate()?",
    "pred_with_args(true)?",
    "unchecked(1, 2, 3)!",
    "add(a, b)$"
};

std::string type_check_samples [] = {
    "x is integer",
    "f(s) is string",
    "foo.bar is list <integer>",
    "no is nothing"
};

////////////////////
//// Test cases
////////////////////

BOOST_AUTO_TEST_SUITE (Operators)

BOOST_AUTO_TEST_CASE(no_infinite_recursion)
{
    auto issues { tao::pegtl::analyze<
        simple_parser<rg::expression>
    >() };

    BOOST_TEST(issues == 0u);
}

BOOST_AUTO_TEST_CASE(boolean_expression)
{
    std::string valid { "1 or 2" };

    BOOST_TEST_MESSAGE("Parsing input " << valid);
    string_input<> in(valid, "test");
    BOOST_TEST(parse<
        simple_parser<rg::boolean_binop>
    >(in) == true);
}

BOOST_AUTO_TEST_CASE(boolean_expression_multiple)
{
    std::string valid { "1 or 2 and a and b" };

    BOOST_TEST_MESSAGE("Parsing input " << valid);
    string_input<> in(valid, "test");
    BOOST_TEST(parse<
        simple_parser<rg::boolean_binop>
    >(in) == true);
}

BOOST_AUTO_TEST_CASE(bitwise_expression)
{
    std::string valid { "a & 0x0000ffff" };

    BOOST_TEST_MESSAGE("Parsing input " << valid);
    string_input<> in(valid, "test");
    BOOST_TEST(parse<
        simple_parser<rg::bitwise_binop>
    >(in) == true);
}

BOOST_AUTO_TEST_CASE(bitwise_expression_multiple)
{
    std::string valid { "1 & (-2 or -3) | +4" };

    BOOST_TEST_MESSAGE("Parsing input " << valid);
    string_input<> in(valid, "test");
    BOOST_TEST(parse<
        simple_parser<rg::bitwise_binop>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(relation_expression, data::make(relation_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::relation_binop>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(shift_expression, data::make(shift_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::shift_binop>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(additive_expression, data::make(addsub_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::additive_binop>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(multiplicative_expression, data::make(muldiv_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::multiplicative_binop>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(exponential_expression, data::make(exp_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::exponential_binop>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(cast_expression, data::make(cast_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::cast_op>
    >(in) == true);
}

BOOST_AUTO_TEST_CASE(ternary_expression)
{
    std::string valid { "if x > 0 then y else z" };

    BOOST_TEST_MESSAGE("Parsing input " << valid);
    string_input<> in(valid, "test");
    BOOST_TEST(parse<
        simple_parser<rg::ternary_op>
    >(in) == true);
}

BOOST_AUTO_TEST_CASE(expression_list)
{
    std::string valid { "1, 2, 3 + 4, x and y" };

    BOOST_TEST_MESSAGE("Parsing input " << valid);
    string_input<> in(valid, "test");
    BOOST_TEST(parse<
        simple_parser<rg::expression_list>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(array_expression, data::make(array_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::array_expression>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(list_expression, data::make(list_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::list_expression>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(tuple_expression, data::make(tuple_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::tuple_expression>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(symbol_list_expression, data::make(symbol_list_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::symbol_list_expression>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(subscript_expression, data::make(subscript_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::postfix_op>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(key_value_pair, data::make(kv_pair_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::kv_pair<rg::identifier>>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(dictionary_expression, data::make(dictionary_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::dictionary_expression>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(member_access_expression, data::make(member_access_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::postfix_op>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(function_call_expression, data::make(function_call_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");
    BOOST_TEST(parse<
        simple_parser<rg::postfix_op>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(type_check_expression, data::make(type_check_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");

    BOOST_TEST(parse<
        simple_parser<rg::type_check_op>
    >(in) == true);
}

BOOST_AUTO_TEST_SUITE_END ()