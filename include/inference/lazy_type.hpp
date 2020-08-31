#ifndef RHEA_INFERENCE_LAZY_TYPE_HPP
#define RHEA_INFERENCE_LAZY_TYPE_HPP

#include <vector>
#include <functional>

#include "../types/types.hpp"
#include "../ast.hpp"
#include "engine_fwd.hpp"

/*
 * We define a "lazy" type inference using a lambda. This way,
 * the type doesn't have to be calculated until it's actually
 * used, and it can be updated as more information is discovered.
 * 
 * One small problem is that lambdas with captures can't be stored
 * as map values, because they don't have an implicit conversion
 * to function pointer. So we'll make an object that holds a lambda
 * without captures, as well as pointers to the type engine and the
 * node we're evaluating.
 */
namespace rhea { namespace inference {
    using namespace rhea::ast;

    struct InferredType
    {
        // A function object representing how to create the type.
        using function_type = std::function<types::TypeInfo(TypeEngine*, ASTNode*)>;

        function_type function = [](TypeEngine*, ASTNode*){ return types::UnknownType(); };

        TypeEngine* engine;
        ASTNode* node;

        types::TypeInfo operator()() { return function(engine, node); }
    };
}}

#endif /* RHEA_INFERENCE_LAZY_TYPE_HPP */