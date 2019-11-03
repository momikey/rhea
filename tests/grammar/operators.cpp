#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <string>

#include <tao/pegtl.hpp>
#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/tracer.hpp>

#include "../../include/grammar/operators.hpp"

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
        simple_parser<rg::expression>
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
    std::string valid { "1 & -2" };

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

BOOST_AUTO_TEST_SUITE_END ()