#ifndef RHEA_CODEGEN_TYPE_CONVERT_HPP
#define RHEA_CODEGEN_TYPE_CONVERT_HPP

#include <memory>

#include <fmt/format.h>

#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>

#include "generator_fwd.hpp"
#include "../ast.hpp"
#include "../types/types.hpp"
#include "../types/to_string.hpp"

/*
 * LLVM IR uses a very strong type system. Even stronger than Rhea's, in fact.
 * In most cases, IR instructions aren't allowed to operate on values that aren't
 * of the exact same type. Thus, we need a way to convert between them.
 * 
 * Rhea does not allow implicit narrowing conversions or conversions from integer
 * to floating-point. As for other builtin types, symbols cannot be converted in
 * any way, vector types (lists, arrays, tuples, and even strings) are...nontrivial,
 * and the `any` and `nothing` types require special handling.
 * 
 * That leaves all numeric types, booleans, references, pointers, and structures,
 * all of which will be accounted for in the free functions declared here. For
 * user-defined, the proper codegen response to a conversion is to call the appropriate
 * function (an overloaded `to$`). For the rest, we can emit the proper LLVM IR,
 * because it's usually only something like a `sext` or `uitofp` instruction.
 */
namespace rhea { namespace codegen {
    using llvm::Value;

    /*
     * The base converter function. We take an optional "explicit" argument (note
     * that we change the name, because `explicit` is a reserved word in C++) to denote
     * cases where we really are making an explicit conversion. This includes the coercion
     * operator `^` and the `as` casting operator.
     */
    Value* convert_type(CodeGenerator* gen, Value* value, types::TypeInfo from, types::TypeInfo to, bool explicit_);

    /*
     * Private helper functions
     */
    namespace internal {
        bool can_implicitly_convert(types::TypeInfo from, types::TypeInfo to);

        // Template for type conversion helpers. These are intended to be called by
        // a variant visitor in `convert_type()` above. It may *look* like there will
        // be N**2 possibilities, but most will end up being some variation on the
        // "find a proper conversion function and call it" theme.
        template <typename From, typename To>
        Value* convert(CodeGenerator* gen, Value* value, From from, To to)
        {
            // TODO: Sensible default, calling a Rhea `to$` overload if possible.
            throw ast::unimplemented_type("Unable to convert types");
        }

        template <>
        Value* convert(CodeGenerator* gen, Value* value, types::SimpleType from, types::SimpleType to);
    }
}}

#endif /* RHEA_CODEGEN_TYPE_CONVERT_HPP */