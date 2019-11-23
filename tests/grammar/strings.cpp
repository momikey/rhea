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

std::string basic_string_samples[] = {
    "'abc'",
    R"("abc")",
    R"("\n")",
    R"("\"")",
    "'123'",
    R"('\x1b')",
    R"("\u03b0")",
    R"("\U0000F042")"
};

BOOST_AUTO_TEST_SUITE (Strings)

BOOST_DATA_TEST_CASE_F(fixture, basic_strings, data::make(basic_string_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");

    BOOST_TEST(parse<
        simple_parser<rg::string_literal>
    >(in) == true);
}

BOOST_AUTO_TEST_SUITE_END ()