#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <string>
#include <memory>
#include <vector>

#include "../../include/codegen/generator.hpp"
#include "../../include/codegen/code_visitor.hpp"
#include "../../include/ast.hpp"

#include "test_setup.hpp"

namespace data = boost::unit_test::data;
namespace cg = rhea::codegen;

namespace {
    struct CodegenFixture
    {
        cg::CodeGenerator gen;
    };
    // Datasets

    // Test cases
    BOOST_FIXTURE_TEST_SUITE (codegen_init, CodegenFixture)

    BOOST_AUTO_TEST_CASE (codegen_creation)
    {
        BOOST_TEST_MESSAGE("Testing LLVM includes");
        BOOST_TEST(1 == 1);
    }

    BOOST_AUTO_TEST_SUITE_END ()
}