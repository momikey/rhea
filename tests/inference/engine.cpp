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

    BOOST_AUTO_TEST_SUITE_END()
}