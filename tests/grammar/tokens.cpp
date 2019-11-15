#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <string>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/tracer.hpp>

#include "test_setup.hpp"

#include "../../include/grammar/tokens.hpp"

namespace data = boost::unit_test::data;

using tao::pegtl::parse;
using tao::pegtl::string_input;
namespace rg = rhea::grammar;

// Datasets

std::string unsigned_integer_samples[] = {"0", "1", "42", "999", "1234567890"};
std::string signed_integer_samples[] = {"0", "+1", "-2", "-1234", "0000004"};
std::string hex_literal_samples[] = {"0x0", "0x1", "0xa", "0xff", "0x0000aaaa00005555"};
std::string float_literal_samples[] {"0.01", "123.456", "-1e6", "+999E-999", "46.29_f"};
std::string identifier_samples[] = {"a", "Abc", "_foo", "bar_42"};
std::string integer_literal_samples[] = {"+7", "3_b", "42_u", "-98765_l", "1000000000_ul"};
std::string symbol_samples[] = {"@foo", "@A", "@a_long_name", "@a1b2"};
std::string fully_qualified_samples[] = { "id", "main:_id", "std:math:sin", ":relative", "a_module:n123:CONSTANT"};

BOOST_AUTO_TEST_SUITE(Integer)

BOOST_DATA_TEST_CASE_F(fixture, unsigned_integer, data::make(unsigned_integer_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");

    BOOST_TEST(parse<
        simple_parser<rg::unsigned_integer>
    >(in) == true);
}

BOOST_AUTO_TEST_CASE(bad_unsigned_integer)
{
    std::string invalid { "1abc" };

    BOOST_TEST_MESSAGE("Parsing invalid input " << invalid);
    string_input<> in(invalid, "test");
    BOOST_TEST(parse<
        simple_parser<rg::unsigned_integer>
    >(in) == false);
}

BOOST_DATA_TEST_CASE_F(fixture, signed_integer, data::make(signed_integer_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");

    BOOST_TEST(parse<
        simple_parser<rg::signed_integer>
    >(in) == true);
}

BOOST_AUTO_TEST_CASE(bad_signed_integer)
{
    std::string invalid { "1abc" };

    BOOST_TEST_MESSAGE("Parsing invalid input " << invalid);
    string_input<> in(invalid, "test");
    BOOST_TEST(parse<
        simple_parser<rg::signed_integer>
    >(in) == false);
}

BOOST_DATA_TEST_CASE_F(fixture, hex_literals, data::make(hex_literal_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");

    BOOST_TEST(parse<
        simple_parser<rg::hex_literal>
    >(in) == true);
}

BOOST_AUTO_TEST_CASE(bad_hex_literal)
{
    std::string invalid { "0x0g" };

    BOOST_TEST_MESSAGE("Parsing invalid input " << invalid);
    string_input<> in(invalid, "test");
    BOOST_TEST(parse<
        simple_parser<rg::hex_literal>
    >(in) == false);
}

BOOST_AUTO_TEST_CASE(incomplete_hex_literal)
{
    std::string invalid { "0x" };

    BOOST_TEST_MESSAGE("Parsing invalid input " << invalid);
    string_input<> in(invalid, "test");
    BOOST_CHECK_THROW(parse<
        simple_parser<rg::hex_literal>
    >(in), tao::pegtl::parse_error);
}

BOOST_DATA_TEST_CASE_F(fixture, float_literals, data::make(float_literal_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");

    BOOST_TEST(parse<
        simple_parser<rg::float_literal>
    >(in) == true);
}

BOOST_AUTO_TEST_CASE(bad_float_literal)
{
    std::string invalid { "0.55a" };

    BOOST_TEST_MESSAGE("Parsing invalid input " << invalid);
    string_input<> in(invalid, "test");
    BOOST_TEST(parse<
        simple_parser<rg::float_literal>
    >(in) == false);
}

BOOST_AUTO_TEST_CASE(boolean_literal)
{
    std::string valid[] { "true", "false" };

    BOOST_TEST_MESSAGE("Parsing valid input " << valid[0]);
    string_input<> in_true(valid[0], "test");
    BOOST_TEST(parse<
        simple_parser<rg::boolean_literal>
    >(in_true) == true); 

    BOOST_TEST_MESSAGE("Parsing valid input " << valid[1]);
    string_input<> in_false(valid[1], "test");
    BOOST_TEST(parse<
        simple_parser<rg::boolean_literal>
    >(in_false) == true); 
}

BOOST_DATA_TEST_CASE_F(fixture, integer_literals, data::make(integer_literal_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");

    BOOST_TEST(parse<
        simple_parser<rg::integer_literal>
    >(in) == true);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Identifier)

BOOST_DATA_TEST_CASE_F(fixture, identifier, data::make(identifier_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");

    BOOST_TEST(parse<
        simple_parser<rg::identifier>
    >(in) == true);
}

BOOST_AUTO_TEST_CASE(bad_identifier)
{
    std::string invalid { "1abc" };

    BOOST_TEST_MESSAGE("Parsing invalid input " << invalid);
    string_input<> in(invalid, "test");
    BOOST_TEST(parse<
        simple_parser<rg::identifier>
    >(in) == false);
}

BOOST_DATA_TEST_CASE_F(fixture, symbol, data::make(symbol_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");

    BOOST_TEST(parse<
        simple_parser<rg::symbol_name>
    >(in) == true);
}

BOOST_AUTO_TEST_CASE(bad_symbol)
{
    std::string invalid { "@@" };

    BOOST_TEST_MESSAGE("Parsing invalid input " << invalid);
    string_input<> in(invalid, "test");
    BOOST_CHECK_THROW(parse<
        simple_parser<rg::symbol_name>
    >(in), tao::pegtl::parse_error);
}

BOOST_DATA_TEST_CASE_F(fixture, fully_qualified_identifier, data::make(fully_qualified_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");

    BOOST_TEST(parse<
        simple_parser<rg::fully_qualified>
    >(in) == true);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Comment)

BOOST_AUTO_TEST_CASE(eol_comment)
{
    std::string valid { "# some comment\n" };

    BOOST_TEST_MESSAGE("Parsing line comment");
    string_input<> in (valid, "test");
    BOOST_TEST(parse<
        simple_parser<rg::line_comment>
    >(in) == true);
}

BOOST_AUTO_TEST_CASE(eof_comment)
{
    std::string valid { "# some comment" };

    BOOST_TEST_MESSAGE("Parsing line comment to EOF");
    string_input<> in (valid, "test");
    BOOST_TEST(parse<
        simple_parser<rg::line_comment>
    >(in) == true);
}

BOOST_AUTO_TEST_CASE(block_comment)
{
    std::string valid { "#{some comment\n\tpossibly with blank lines\n#}" };

    BOOST_TEST_MESSAGE("Parsing block comment");
    string_input<> in (valid, "test");
    BOOST_TEST(parse<
        simple_parser<rg::block_comment>
    >(in) == true);
}

BOOST_AUTO_TEST_CASE(incomplete_block_comment)
{
    std::string invalid { "#{this comment doesn't end before EOF" };

    BOOST_TEST_MESSAGE("Parsing invalid block comment");
    string_input<> in (invalid, "test");
    BOOST_CHECK_THROW(parse<
        simple_parser<rg::block_comment>
    >(in), tao::pegtl::parse_error);
}

BOOST_AUTO_TEST_SUITE_END()
