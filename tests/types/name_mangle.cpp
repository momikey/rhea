#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/algorithm/string/split.hpp>

#include <string>
#include <vector>
#include <memory>
#include <exception>

#include "../../include/types/types.hpp"
#include "../../include/types/to_string.hpp"
#include "../../include/types/name_mangle.hpp"

namespace data = boost::unit_test::data;
namespace util = rhea::util;

namespace {
    using namespace rhea::types;

    // Datasets

    // Test cases
    BOOST_AUTO_TEST_SUITE (Name_mangler)

    BOOST_AUTO_TEST_CASE (simple_function_no_args)
    {
        BOOST_TEST_MESSAGE("Testing mangling of simple function without arguments or specified return type");
        FunctionType ft {};

        auto mangled = mangle_function_name("foo", ft);
        BOOST_TEST(mangled == "_Rf3foov0");
    }
    
    BOOST_AUTO_TEST_SUITE_END ()
}