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

BOOST_AUTO_TEST_SUITE_END ()
