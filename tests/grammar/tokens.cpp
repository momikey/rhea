#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <string>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/tracer.hpp>

#include "../../include/grammar/tokens.hpp"

namespace data = boost::unit_test::data;

using tao::pegtl::parse;
using tao::pegtl::string_input;
namespace rg = rhea::grammar;

struct fixture
{
    fixture() = default;
    ~fixture() = default;

    void setup() {}
    void teardown() {}
};

// Wrapper for parser + EOR
template<typename T>
struct simple_parser : tao::pegtl::seq<
    T, tao::pegtl::eolf> {};

// Datasets

std::string unsigned_integer_samples[] = {"0", "1", "42", "999", "1234567890"};
std::string signed_integer_samples[] = {"0", "+1", "-2", "-1234", "0000004"};
std::string identifier_samples[] = {"a", "Abc", "_foo", "bar_42"};

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

BOOST_AUTO_TEST_SUITE_END()