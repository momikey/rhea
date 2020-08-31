#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <string>

#include <tao/pegtl.hpp>
#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/tracer.hpp>

#include "test_setup.hpp"

#include "../../include/grammar/module.hpp"

namespace data = boost::unit_test::data;

namespace {
    using tao::pegtl::parse;
    using tao::pegtl::string_input;
    namespace rg = rhea::grammar;

    ////////////////////
    //// Sample data
    ////////////////////

    std::string module_example =
    "### my_module.rhea ###\n"
    "\n"
    "module my_module;\n"
    "\n"
    "def my_function = {\n"
    "	# ...some function...\n"
    "   return true;\n"
    "}\n"
    "\n"
    "export { my_function };\n";

    std::string program_example =
    "### main.rhea ###\n"
    "\n"
    "use std:basic; # to test/illustrate use statement\n"
    "\n"
    "def main = {\n"
    "    print('Hello, world!\\n');\n"
    "}\n";

    ////////////////////
    //// Test cases
    ////////////////////

    BOOST_AUTO_TEST_SUITE (Analysis)

    BOOST_AUTO_TEST_CASE(no_infinite_recursion_module)
    {
        auto issues { tao::pegtl::analyze<
            simple_parser<rg::module_definition>
        >() };

        BOOST_TEST(issues == 0u);
    }

    BOOST_AUTO_TEST_CASE(no_infinite_recursion_program)
    {
        auto issues { tao::pegtl::analyze<
            simple_parser<rg::program_definition>
        >() };

        BOOST_TEST(issues == 0u);
    }

    BOOST_AUTO_TEST_SUITE_END ()

    BOOST_AUTO_TEST_SUITE (Module_and_program)

    BOOST_AUTO_TEST_CASE(module_definition)
    {
        BOOST_TEST_MESSAGE("Parsing " << module_example);
        string_input<> in(module_example, "test");

        BOOST_TEST(parse<
            simple_parser<rg::module_definition>
        >(in) == true);
    }

    BOOST_AUTO_TEST_CASE(program_definition)
    {
        BOOST_TEST_MESSAGE("Parsing " << program_example);
        string_input<> in(program_example, "test");

        BOOST_TEST(parse<
            simple_parser<rg::program_definition>
        >(in) == true);
    }

    BOOST_AUTO_TEST_SUITE_END ()
}