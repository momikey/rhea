#ifndef RHEA_INFERENCE_ENGINE_HPP
#define RHEA_INFERENCE_ENGINE_HPP

#include <unordered_map>
#include <string>
#include <vector>

#include "ast.hpp"
#include "types/types.hpp"
#include "types/mapper.hpp"

#include "lazy_type.hpp"
#include "visitor.hpp"

/*
 * The type inference engine tracks every inferred type in a Rhea program
 * or module. This includes expressions, variable declarations, constant
 * declarations, function return values, argument values, calls, aliases,
 * and much more.
 * 
 * Note that this is technically not part of the type system, so we put it
 * in its own namespace, but it so heavily relies on rhea::types that we'll
 * just use a `using` on the whole thing.
 */
namespace rhea { namespace inference {
    using namespace rhea::types;

    struct TypeEngine
    {
        TypeEngine();

        /*
         * The core of the inference engine is a map connecting AST node
         * pointers to the types that have been inferred.
         * 
         * However, this doesn't quite work.
        */
        std::unordered_map<ast::ASTNode*, InferredType> inferred_types;
        TypeMapper mapper;

        InferenceVisitor visitor;

        friend InferenceVisitor;
    };
}}

#endif /* RHEA_INFERENCE_ENGINE_HPP */