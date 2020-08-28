#ifndef RHEA_NODES_CONCEPT_HPP
#define RHEA_NODES_CONCEPT_HPP

#include <memory>
#include <string>
#include <vector>

#include "node_base.hpp"
#include "identifiers.hpp"
#include "typenames.hpp"

#include "util/serialize_array.hpp"

/*
 * AST Nodes for concepts and related syntax.
 */
namespace rhea { namespace ast {
    // The base concept match is just a type pair, but we use a different
    // node type for it so we can keep the distinction where needed.
    class ConceptMatch : public ASTNode
    {
        public:
        ConceptMatch(std::string n, std::unique_ptr<Typename> t)
            : name(n), concept_type(std::move(t)) {}

        // We store the name as a string because we know it's a local
        // identifier. Also, we can't use "concept" as the name for the
        // typename node, because it's an actual keyword starting in C++20,
        // and we want to be forward-compatible.
        std::string name;
        std::unique_ptr<Typename> concept_type;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override
            { return fmt::format("(ConceptMatch,{0},{1})", name, concept_type->to_string()); }
    };

    // A member check is just an operation to see if a given type is
    // a structure containing a field with the given name.
    class MemberCheck : public ASTNode
    {
        public:
        MemberCheck(std::string t, std::string m)
            : type(t), member(m) {}

        std::string type;
        std::string member;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override
            { return fmt::format("(MemberCheck,{0},{1})", type, member); }
    };

    // A function check is a compile-time operation to see if, given a type T,
    // there exists a valid function F taking that type as an argument.
    class FunctionCheck : public ASTNode
    {
        public:
        FunctionCheck(std::string t, std::unique_ptr<AnyIdentifier> fn,
            child_vector<Typename>& fas, std::unique_ptr<Typename> rtn);

        std::string type_name;
        std::unique_ptr<AnyIdentifier> function_name;
        child_vector<Typename> function_arguments;
        std::unique_ptr<Typename> return_type_name;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override;
    };
}}


#endif /* RHEA_NODES_CONCEPT_HPP */