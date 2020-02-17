#ifndef RHEA_INFERENCE_LAZY_TYPE_HPP
#define RHEA_INFERENCE_LAZY_TYPE_HPP

#include <vector>
#include <functional>

#include "../types/types.hpp"

/*
 * We define a "lazy" type inference using a lambda. This way,
 * the type doesn't have to be calculated until it's actually
 * used, and it can be updated as more information is discovered.
 * 
 * One small problem is that lambdas with captures can't be stored
 * as map values, because they don't have an implicit conversion
 * to function pointer. So we'll make an object that holds a lambda
 * without captures, plus an argument vector. (Variadic templates
 * create new types which again prevents us from directly using the map.)
 */
namespace rhea { namespace inference {
    struct InferredType
    {
        // A function object representing how to create the type.
        using function_type = std::function<types::TypeInfo(std::vector<types::TypeInfo>)>;

        function_type function = [](std::vector<types::TypeInfo> ti){ return types::UnknownType(); };
        std::vector<types::TypeInfo> type_arguments;

        types::TypeInfo operator()() { return function(type_arguments); }
    };
}}

#endif /* RHEA_INFERENCE_LAZY_TYPE_HPP */