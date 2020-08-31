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
#include "../../include/types/mapper.hpp"

namespace data = boost::unit_test::data;
namespace types = rhea::types;
namespace util = rhea::util;

namespace {
    // Fixture to create a type mapper for each test case
    struct MapperFixture
    {
        types::TypeMapper mapper;
    };
    // Datasets

    // Test cases
    BOOST_FIXTURE_TEST_SUITE (Type_mapper, MapperFixture)

    BOOST_AUTO_TEST_CASE (mapper_create)
    {
        BOOST_TEST_MESSAGE("Creating type mapper");
        BOOST_TEST(1 == 1);
    }

    BOOST_AUTO_TEST_CASE (builtin_type)
    {
        auto integer_type = mapper.get_type_for("integer").type();

        auto as_simple = util::get_if<types::SimpleType>(&integer_type);

        BOOST_TEST_MESSAGE("Checking builtin type 'integer'");
        BOOST_TEST(as_simple != nullptr);
        BOOST_TEST((as_simple->type == types::BasicType::Integer));
    }

    BOOST_AUTO_TEST_CASE (bad_type)
    {
        auto bad_type = mapper.get_type_for("bad").type();

        BOOST_TEST_MESSAGE("Checking bad typename");
        BOOST_TEST((util::get_if<types::UnknownType>(&bad_type) != nullptr));
    }

    BOOST_AUTO_TEST_CASE (insert_type)
    {
        BOOST_TEST_MESSAGE("Inserting definition for type 'myint'");

        // Create what's basically an alias for integers
        auto new_int_type = types::SimpleType(types::BasicType::Integer, true, true);

        // Insert into the type mapper
        auto result = mapper.add_type_definition("myint", new_int_type);

        BOOST_TEST(result);

        // Now ensure that it was added
        auto added = mapper.get_type_for("myint").type();
        auto as_simple = util::get_if<types::SimpleType>(&added);

        BOOST_TEST(as_simple != nullptr);
        BOOST_TEST((as_simple->type == types::BasicType::Integer));
    }

    BOOST_AUTO_TEST_CASE (remove_type)
    {
        BOOST_TEST_MESSAGE("Inserting a type definition for 'myint', then removing it");

        // Same code as above, but without the checks we did there
        auto new_int_type = types::SimpleType(types::BasicType::Integer, true, true);
        auto result = mapper.add_type_definition("myint", new_int_type);

        if (result)
        {
            auto removed = mapper.remove_type_definition("myint");
            BOOST_TEST(removed.has_value());

            auto after = mapper.get_type_for("myint").type();
            BOOST_TEST((util::get_if<types::UnknownType>(&after) != nullptr));
        }
        else
        {
            // This shouldn't happen
            throw std::invalid_argument("Mapper did not add new type to map");
        }
    }

    BOOST_AUTO_TEST_CASE (insert_function_type)
    {
        BOOST_TEST_MESSAGE("Inserting a function type definition, taking an integer 'i' and returning nothing");

        auto fn_type = types::FunctionType {
            { { "i", std::make_shared<types::TypeInfo>(types::SimpleType(types::BasicType::Integer, true, true)) } },
            std::make_shared<types::TypeInfo>(types::NothingType())
        };

        auto result = mapper.add_type_definition("f <integer> -> nothing", fn_type);

        if (result)
        {
            auto inserted = mapper.get_type_for("f <integer> -> nothing").type();
            BOOST_TEST((util::get_if<types::FunctionType>(&inserted) != nullptr));
        }
        else
        {
            // This shouldn't happen
            throw std::invalid_argument("Mapper did not add new type to map");
        }
    }

    BOOST_AUTO_TEST_CASE (insert_optional_type)
    {
        BOOST_TEST_MESSAGE("Inserting an optional type holding an integer");

        auto opt_type = types::OptionalType { std::make_shared<types::TypeInfo>(mapper.get_type_for("integer")) };

        auto result = mapper.add_type_definition("integer?", opt_type);

        if (result)
        {
            auto inserted = mapper.get_type_for("integer?").type();
            BOOST_TEST((util::get_if<types::OptionalType>(&inserted) != nullptr));
        }
        else
        {
            // This shouldn't happen
            throw std::invalid_argument("Mapper did not add new type to map");
        }
    }

    BOOST_AUTO_TEST_CASE (insert_variant_type)
    {
        BOOST_TEST_MESSAGE("Inserting a variant over integer and string");

        auto var_type = types::VariantType {{
            std::make_shared<types::TypeInfo>(mapper.get_type_for("integer")),
            std::make_shared<types::TypeInfo>(mapper.get_type_for("string"))
        }};

        auto result = mapper.add_type_definition("|integer, string|", var_type);

        if (result)
        {
            auto inserted = mapper.get_type_for("|integer, string|").type();
            BOOST_TEST((util::get_if<types::VariantType>(&inserted) != nullptr));
        }
        else
        {
            // This shouldn't happen
            throw std::invalid_argument("Mapper did not add new type to map");
        } 
    }

    BOOST_AUTO_TEST_CASE (insert_structure_type)
    {
        BOOST_TEST_MESSAGE("Inserting a structure St with fields i (integer) and s (string)");

        auto struct_type = types::StructureType {{
            { "i", std::make_shared<types::TypeInfo>(mapper.get_type_for("integer"))},
            { "s", std::make_shared<types::TypeInfo>(mapper.get_type_for("string"))}
        }};

        auto result = mapper.add_type_definition("St", struct_type);

        if (result)
        {
            auto inserted = mapper.get_type_for("St").type();
            BOOST_TEST((util::get_if<types::StructureType>(&inserted) != nullptr));
        }
        else
        {
            // This shouldn't happen
            throw std::invalid_argument("Mapper did not add new type to map");
        } 
    }

    BOOST_AUTO_TEST_SUITE_END ()
}