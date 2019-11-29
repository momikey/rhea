#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <type_traits>
#include <typeinfo>

#include "../../include/ast.hpp"

#include "test_setup.hpp"

namespace data = boost::unit_test::data;

// Datasets

int32_t integer_samples[] = { 0, 1, -42, 69, 32768, 0x7fffffff };
int8_t byte_samples[] = { 0, 1, -1, 127, -128 };
float float_samples[] = { 1.0, 0.1, -3.1e13, 1.0 / 3.0 };
double double_samples[] = { 1.0, 0.00000123, 1e99, -2.56e-16 };
int64_t long_samples[] = { 0, 2, -4, 0x123456789abcdef };
uint32_t uinteger_samples[] = { 0, 1, 15, 255, 0xffffffff };
uint8_t ubyte_samples[] = { 0, 13, 42, 100, 212 };
uint64_t ulong_samples[] = { 0, 999, 2020, 1L << 48, 0xffffffffffffffff };

BOOST_AUTO_TEST_SUITE (AST_Numeric)

BOOST_DATA_TEST_CASE(integer_ast, data::make(integer_samples))
{
    auto node = rhea::ast::Integer(sample);

    BOOST_TEST_MESSAGE("Testing AST Node " << node.to_string());

    BOOST_TEST(node.value == sample);
    BOOST_TEST(node.type == rhea::ast::Numeric::Integer);
}

BOOST_DATA_TEST_CASE(byte_ast, data::make(byte_samples))
{
    auto node = rhea::ast::Byte(sample);

    BOOST_TEST_MESSAGE("Testing AST Node " << node.to_string());

    BOOST_TEST(node.value == sample);
    BOOST_TEST(node.type == rhea::ast::Numeric::Byte);
}

BOOST_DATA_TEST_CASE(float_ast, data::make(float_samples))
{
    auto node = rhea::ast::Float(sample);

    BOOST_TEST_MESSAGE("Testing AST Node " << node.to_string());

    BOOST_TEST(node.value == sample);
    BOOST_TEST(node.type == rhea::ast::Numeric::Float);
}

BOOST_DATA_TEST_CASE(double_ast, data::make(double_samples))
{
    auto node = rhea::ast::Double(sample);

    BOOST_TEST_MESSAGE("Testing AST Node " << node.to_string());

    BOOST_TEST(node.value == sample);
    BOOST_TEST(node.type == rhea::ast::Numeric::Double);
}

BOOST_DATA_TEST_CASE(long_ast, data::make(long_samples))
{
    auto node = rhea::ast::Long(sample);

    BOOST_TEST_MESSAGE("Testing AST Node " << node.to_string());

    BOOST_TEST(node.value == sample);
    BOOST_TEST(node.type == rhea::ast::Numeric::Long);
}

BOOST_DATA_TEST_CASE(uinteger_ast, data::make(uinteger_samples))
{
    auto node = rhea::ast::UnsignedInteger(sample);

    BOOST_TEST_MESSAGE("Testing AST Node " << node.to_string());

    BOOST_TEST(node.value == sample);
    BOOST_TEST(node.type == rhea::ast::Numeric::UnsignedInteger);
}

BOOST_DATA_TEST_CASE(ubyte_ast, data::make(ubyte_samples))
{
    auto node = rhea::ast::UnsignedByte(sample);

    BOOST_TEST_MESSAGE("Testing AST Node " << node.to_string());

    BOOST_TEST(node.value == sample);
    BOOST_TEST(node.type == rhea::ast::Numeric::UnsignedByte);
}

BOOST_DATA_TEST_CASE(ulong_ast, data::make(ulong_samples))
{
    auto node = rhea::ast::UnsignedLong(sample);

    BOOST_TEST_MESSAGE("Testing AST Node " << node.to_string());

    BOOST_TEST(node.value == sample);
    BOOST_TEST(node.type == rhea::ast::Numeric::UnsignedLong);
}

BOOST_AUTO_TEST_SUITE_END ()