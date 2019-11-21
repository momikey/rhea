#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <string>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/tracer.hpp>

#include "test_setup.hpp"

#include "../../include/grammar/tokens.hpp"
#include "../../include/grammar/typenames.hpp"
#include "../../include/grammar/operators.hpp"
#include "../../include/grammar/concepts.hpp"

namespace data = boost::unit_test::data;

using tao::pegtl::parse;
using tao::pegtl::string_input;
namespace rg = rhea::grammar;

// Datasets

std::string concept_match_samples[] = {
    "T ~> Iterable",
    "Ty ~> Stringable"
};

std::string concept_function_samples[] = {
    "MyType => foo <MyType> -> boolean",
    "Ty => to$ <Ty> -> string"
};

std::string concept_member_samples[] = {
    "Ty .= foo",
    "Person .= name"
};

std::string concept_block_samples[] = {
    "{ Ty => to$ <Ty> -> string, Ty .= foo }"
};

BOOST_AUTO_TEST_SUITE (Concepts)

BOOST_DATA_TEST_CASE(concept_match, data::make(concept_match_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");

    BOOST_TEST(parse<
        simple_parser<rg::concept_match>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(concept_function_check, data::make(concept_function_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");

    BOOST_TEST(parse<
        simple_parser<rg::concept_function_check>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(concept_member_check, data::make(concept_member_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");

    BOOST_TEST(parse<
        simple_parser<rg::concept_member_check>
    >(in) == true);
}

BOOST_DATA_TEST_CASE(concept_block, data::make(concept_block_samples))
{
    BOOST_TEST_MESSAGE("Parsing " << sample);
    string_input<> in(sample, "test");

    BOOST_TEST(parse<
        simple_parser<rg::concept_block>
    >(in) == true);
}

BOOST_AUTO_TEST_SUITE_END ()