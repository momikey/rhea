#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <string>
#include <memory>
#include <vector>

#include "../../include/inference/engine.hpp"
#include "../../include/ast.hpp"
#include "../../include/util/compat.hpp"

namespace data = boost::unit_test::data;
namespace util = rhea::util;

namespace {
    using namespace rhea::inference;
    using namespace rhea::ast;
    using namespace rhea::types;

    struct InferenceFixture
    {
        TypeEngine engine;
    };

    BOOST_FIXTURE_TEST_SUITE (inference_engine, InferenceFixture)

    BOOST_AUTO_TEST_CASE (engine_creation)
    {
        BOOST_TEST_MESSAGE("Testing type inference engine creation");
        BOOST_TEST(1 == 1);
    }

    BOOST_AUTO_TEST_CASE (infer_boolean_literal)
    {
        BOOST_TEST_MESSAGE("Testing inference of boolean literal");

        std::unique_ptr<ASTNode> node = make_expression<Boolean>(true);

        node->visit(&engine.visitor);

        auto inferred = engine.inferred_types[node.get()]();
        auto as_simple = util::get_if<SimpleType>(&inferred.type());
        BOOST_TEST((as_simple != nullptr));
        BOOST_TEST((as_simple->type == BasicType::Boolean));
    }

    BOOST_AUTO_TEST_CASE (infer_integer_literal)
    {
        BOOST_TEST_MESSAGE("Testing inference of integer literal");

        std::unique_ptr<ASTNode> node = make_expression<Integer>(42);

        node->visit(&engine.visitor);

        auto inferred = engine.inferred_types[node.get()]();
        auto as_simple = util::get_if<SimpleType>(&inferred.type());
        BOOST_TEST((as_simple != nullptr));
        BOOST_TEST((as_simple->type == BasicType::Integer));
    }

    BOOST_AUTO_TEST_CASE (infer_byte_literal)
    {
        BOOST_TEST_MESSAGE("Testing inference of byte literal");

        std::unique_ptr<ASTNode> node = make_expression<Byte>(42);

        node->visit(&engine.visitor);

        auto inferred = engine.inferred_types[node.get()]();
        auto as_simple = util::get_if<SimpleType>(&inferred.type());
        BOOST_TEST((as_simple != nullptr));
        BOOST_TEST((as_simple->type == BasicType::Byte));
    }

    BOOST_AUTO_TEST_CASE (infer_long_literal)
    {
        BOOST_TEST_MESSAGE("Testing inference of long literal");

        std::unique_ptr<ASTNode> node = make_expression<Long>(42L);

        node->visit(&engine.visitor);

        auto inferred = engine.inferred_types[node.get()]();
        auto as_simple = util::get_if<SimpleType>(&inferred.type());
        BOOST_TEST((as_simple != nullptr));
        BOOST_TEST((as_simple->type == BasicType::Long));
    }

    BOOST_AUTO_TEST_CASE (infer_uinteger_literal)
    {
        BOOST_TEST_MESSAGE("Testing inference of unsigned integer literal");

        std::unique_ptr<ASTNode> node = make_expression<UnsignedInteger>(42);

        node->visit(&engine.visitor);

        auto inferred = engine.inferred_types[node.get()]();
        auto as_simple = util::get_if<SimpleType>(&inferred.type());
        BOOST_TEST((as_simple != nullptr));
        BOOST_TEST((as_simple->type == BasicType::UnsignedInteger));
    }

    BOOST_AUTO_TEST_CASE (infer_ubyte_literal)
    {
        BOOST_TEST_MESSAGE("Testing inference of unsigned byte literal");

        std::unique_ptr<ASTNode> node = make_expression<UnsignedByte>(42);

        node->visit(&engine.visitor);

        auto inferred = engine.inferred_types[node.get()]();
        auto as_simple = util::get_if<SimpleType>(&inferred.type());
        BOOST_TEST((as_simple != nullptr));
        BOOST_TEST((as_simple->type == BasicType::UnsignedByte));
    }

    BOOST_AUTO_TEST_CASE (infer_ulong_literal)
    {
        BOOST_TEST_MESSAGE("Testing inference of unsigned long literal");

        std::unique_ptr<ASTNode> node = make_expression<UnsignedLong>(42L);

        node->visit(&engine.visitor);

        auto inferred = engine.inferred_types[node.get()]();
        auto as_simple = util::get_if<SimpleType>(&inferred.type());
        BOOST_TEST((as_simple != nullptr));
        BOOST_TEST((as_simple->type == BasicType::UnsignedLong));
    }

    BOOST_AUTO_TEST_CASE (infer_float_literal)
    {
        BOOST_TEST_MESSAGE("Testing inference of float literal");

        std::unique_ptr<ASTNode> node = make_expression<Float>(3.14f);

        node->visit(&engine.visitor);

        auto inferred = engine.inferred_types[node.get()]();
        auto as_simple = util::get_if<SimpleType>(&inferred.type());
        BOOST_TEST((as_simple != nullptr));
        BOOST_TEST((as_simple->type == BasicType::Float));
    }

    BOOST_AUTO_TEST_CASE (infer_double_literal)
    {
        BOOST_TEST_MESSAGE("Testing inference of double literal");

        std::unique_ptr<ASTNode> node = make_expression<Double>(3.14);

        node->visit(&engine.visitor);

        auto inferred = engine.inferred_types[node.get()]();
        auto as_simple = util::get_if<SimpleType>(&inferred.type());
        BOOST_TEST((as_simple != nullptr));
        BOOST_TEST((as_simple->type == BasicType::Double));
    }

    BOOST_AUTO_TEST_CASE (infer_symbol_literal)
    {
        BOOST_TEST_MESSAGE("Testing inference of symbol literal");

        std::unique_ptr<ASTNode> node = make_expression<Symbol>("foo");

        node->visit(&engine.visitor);

        auto inferred = engine.inferred_types[node.get()]();
        auto as_simple = util::get_if<SimpleType>(&inferred.type());
        BOOST_TEST((as_simple != nullptr));
        BOOST_TEST((as_simple->type == BasicType::Symbol));
    }

    BOOST_AUTO_TEST_CASE (infer_nothing_literal)
    {
        BOOST_TEST_MESSAGE("Testing inference of nothing literal");

        std::unique_ptr<ASTNode> node = make_expression<Nothing>();

        node->visit(&engine.visitor);

        auto inferred = engine.inferred_types[node.get()]();
        BOOST_TEST((util::get_if<NothingType>(&inferred.type()) != nullptr));
    }

    BOOST_AUTO_TEST_CASE (infer_binop)
    {
        BOOST_TEST_MESSAGE("Testing inference of binary operation");

        auto l = make_expression<Integer>(42);
        auto r = make_expression<Integer>(69);
        std::unique_ptr<ASTNode> node = make_expression<BinaryOp>(
            BinaryOperators::Add,
            std::move(l),
            std::move(r)
        );

        node->visit(&engine.visitor);

        auto inferred = engine.inferred_types[node.get()]();
        auto as_simple = util::get_if<SimpleType>(&inferred.type());
        BOOST_TEST((as_simple != nullptr));
        BOOST_TEST((as_simple->type == BasicType::Integer));
    }

    BOOST_AUTO_TEST_CASE (infer_ternary_op)
    {
        BOOST_TEST_MESSAGE("Testing inference of ternary operation");

        auto t = make_expression<Integer>(42);
        auto f = make_expression<Integer>(69);
        auto c = make_expression<Identifier>("foo");
        std::unique_ptr<ASTNode> node = make_expression<TernaryOp>(
            std::move(c),
            std::move(t),
            std::move(f)
        );

        node->visit(&engine.visitor);

        auto inferred = engine.inferred_types[node.get()]();
        auto as_simple = util::get_if<SimpleType>(&inferred.type());
        BOOST_TEST((as_simple != nullptr));
        BOOST_TEST((as_simple->type == BasicType::Integer));
    }

    BOOST_AUTO_TEST_CASE (infer_unary_op)
    {
        BOOST_TEST_MESSAGE("Testing inference of unary operation");

        auto o = make_expression<Integer>(42);
        std::unique_ptr<ASTNode> node = make_expression<UnaryOp>(
            UnaryOperators::Minus,
            std::move(o)
        );

        node->visit(&engine.visitor);

        auto inferred = engine.inferred_types[node.get()]();
        auto as_simple = util::get_if<SimpleType>(&inferred.type());
        BOOST_TEST((as_simple != nullptr));
        BOOST_TEST((as_simple->type == BasicType::Integer));
    }

    BOOST_AUTO_TEST_CASE (infer_variable_definition)
    {
        BOOST_TEST_MESSAGE("Testing inference of variable definition");

        auto l = std::make_unique<Identifier>("foo");
        auto r = make_expression<Integer>(69);
        std::unique_ptr<ASTNode> node = make_statement<Variable>(
            std::move(l),
            std::move(r)
        );

        node->visit(&engine.visitor);

        auto inferred = engine.inferred_types[node.get()]();
        auto as_simple = util::get_if<SimpleType>(&inferred.type());
        BOOST_TEST((as_simple != nullptr));
        BOOST_TEST((as_simple->type == BasicType::Integer));
    }

    BOOST_AUTO_TEST_CASE (infer_constant_definition)
    {
        BOOST_TEST_MESSAGE("Testing inference of constant definition");

        auto l = std::make_unique<Identifier>("foo");
        auto r = make_expression<Integer>(69);
        std::unique_ptr<ASTNode> node = make_statement<Constant>(
            std::move(l),
            std::move(r)
        );

        node->visit(&engine.visitor);

        auto inferred = engine.inferred_types[node.get()]();
        auto as_simple = util::get_if<SimpleType>(&inferred.type());
        BOOST_TEST((as_simple != nullptr));
        BOOST_TEST((as_simple->type == BasicType::Integer));
    }

    BOOST_AUTO_TEST_SUITE_END()
}