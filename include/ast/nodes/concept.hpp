#ifndef RHEA_NODES_CONCEPT_HPP
#define RHEA_NODES_CONCEPT_HPP

#include <memory>
#include <string>
#include <vector>

#include "node_base.hpp"
#include "identifiers.hpp"
#include "typenames.hpp"

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
}}


#endif /* RHEA_NODES_CONCEPT_HPP */