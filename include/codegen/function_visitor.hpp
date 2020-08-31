#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "../ast.hpp"
#include "../types/types.hpp"
#include "../util/compat.hpp"
#include "../visitor/visitor.hpp"
#include "../visitor/default.hpp"
#include "generator_fwd.hpp"
#include "type_convert.hpp"

/*
 * The FunctionVisitor is a "sub-visitor" used to aid in type inference and
 * to declare functions whose first use appears before their definition.
 */
namespace rhea { namespace codegen {
    using util::any;
    using namespace rhea::ast;

    struct FunctionVisitor : visitor::DefaultVisitor
    {
        using argument_map = std::map<std::string, types::TypeInfo>;

        FunctionVisitor(types::TypeInfo rt, argument_map am)
            : return_type(rt), argument_types(am) {}
        
        FunctionVisitor(types::TypeInfo rt) : FunctionVisitor(rt, {}) {}
        FunctionVisitor(argument_map& am) : FunctionVisitor(types::UnknownType(), am) {}
        FunctionVisitor() : FunctionVisitor(types::UnknownType(), {}) {}

        // We only need to override the visitor methods that can possibly
        // lead to a return value. Note that, although these return "any"s,
        // almost all will actually return nothing, instead tracking the
        // resulting return type in the state variable below.

        any visit(If*) override;
        any visit(While*) override;
        any visit(For*) override;
        any visit(With*) override;
        any visit(Match*) override;
        any visit(On*) override;
        any visit(When*) override;
        any visit(TypeCase*) override;
        any visit(Default*) override;
        any visit(Def*) override;
        any visit(GenericDef*) override;
        any visit(Return*) override;
        any visit(Block* n) override;
        any visit(Try* n) override;
        any visit(Catch* n) override;
        any visit(Finally* n) override;

        // We keep track of all the return types we've found while traversing
        // the AST. If these don't match, or if they're not the same as the one
        // the function's definition gives, then we'll have to give an error.
        types::TypeInfo return_type;
        std::vector<types::TypeInfo> potential_return_types;

        // The arguments of the function may come in handy.
        argument_map argument_types;
    };
}}