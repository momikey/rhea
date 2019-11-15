#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <string>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/tracer.hpp>

#include "test_setup.hpp"

#include "../../include/grammar/typenames.hpp"
#include "../../include/grammar/operators.hpp"

namespace data = boost::unit_test::data;

using tao::pegtl::parse;
using tao::pegtl::string_input;
namespace rg = rhea::grammar;

// Datasets

std::string type_pair_samples[] = {
    "s : string",
    "int : integer",
    "f : std:fs:File"
};

std::string generic_type_samples[] = {
    "list <string>",
    "array <integer>",
    "foo <bar,baz>"
};

std::string array_type_samples[] = {
    "integer[10]",
    "string[42]",
    "std:fs:File[n]"
};

std::string complex_type_samples[] = {
    "list <string> [5]",
    "dict <integer>[n*2]"
};

BOOST_AUTO_TEST_SUITE (Typenames)

BOOST_DATA_TEST_CASE(type_pair, data::make(type_pair_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");

    BOOST_TEST(parse<
        simple_parser<rg::type_pair>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(generic_type, data::make(generic_type_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");

    BOOST_TEST(parse<
        simple_parser<rg::type_name>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(array_type, data::make(array_type_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");

    BOOST_TEST(parse<
        simple_parser<rg::type_name>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(complex_type, data::make(complex_type_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");

    BOOST_TEST(parse<
        simple_parser<rg::type_name>
    >(in) == true);
}

BOOST_AUTO_TEST_SUITE_END ()