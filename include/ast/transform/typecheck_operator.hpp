#ifndef RHEA_TRANSFORM_TYPECHECK_HPP
#define RHEA_TRANSFORM_TYPECHECK_HPP

#include <memory>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

#include "../../grammar.hpp"

namespace rhea { namespace ast {
    using namespace tao::pegtl;

    /*
     * Transformer for binary operations where the RHS is a type.
     * This includes the "as" and "is" operators.
     * 
     * The reason this is a separate transformer, rather than using
     * the general binary op rearranger, is that we might want to
     * do some more work on typenames before putting them in the AST.
     * (At the moment, it's the same code, but that may change later.)
     */

    struct typecheck_rearrange : parse_tree::apply<typecheck_rearrange>
    {
        template< typename NodeType, typename... States >
        static void transform( std::unique_ptr<NodeType>& n, States&&... st )
        {
            if (n->children.size() == 1)
            {
                // If the expression only has 1 child, fold that child
                // into the parent.
                n = std::move(n->children.back());
            }
            else
            {
                // Otherwise, pull out the operator and make it the new
                // parent of the LHS and RHS, then recurse through those.
                n->remove_content();

                auto& c = n->children;

                auto rhs = std::move(c.back());
                c.pop_back();

                auto op = std::move(c.back());
                c.pop_back();

                // This node becomes the LHS of the operator
                op->children.emplace_back(std::move(n));
                op->children.emplace_back(std::move(rhs));

                n = std::move(op);
                transform(n->children.front(), st...);
            }
        }
    };
}}

#endif /* RHEA_TRANSFORM_TYPECHECK_HPP */