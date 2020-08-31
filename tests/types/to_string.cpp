#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/algorithm/string/split.hpp>

#include <string>
#include <vector>
#include <memory>

#include "../../include/types/types.hpp"
#include "../../include/types/to_string.hpp"
#include "../../include/ast.hpp"

namespace data = boost::unit_test::data;
namespace ast = rhea::ast;
namespace types = rhea::types;
namespace util = rhea::util;

namespace {
    // Datasets

    // Test cases
    BOOST_AUTO_TEST_SUITE (Type_support)

    BOOST_AUTO_TEST_CASE (typeinfo_string)
    {
        types::TypeInfo ti = types::SimpleType(types::BasicType::Integer, true, true);

        BOOST_TEST_MESSAGE("Testing typeinfo string output");

        auto result = types::to_string(ti);

        BOOST_TEST_MESSAGE(result);
        BOOST_TEST((result == "integer"));
    }

    BOOST_AUTO_TEST_SUITE_END ()
}