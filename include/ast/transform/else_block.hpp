#ifndef RHEA_TRANSFORM_ELSE_HPP
#define RHEA_TRANSFORM_ELSE_HPP

#include <memory>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

#include "../../grammar.hpp"

namespace rhea { namespace ast {
    using namespace tao::pegtl;

    /*
     * This transformer folds an else block's children into its place or,
     * if it has no children (i.e., there is no else block present), deletes
     * the node entirely.
     */

    struct else_rearrange : parse_tree::apply<else_rearrange>
    {
        template< typename NodeType, typename... States >
        static void transform( std::unique_ptr<NodeType>& n, States&&... st )
        {
            if (n->children.empty())
            {
                // If there aren't any children, delete this node, because it's
                // an artifact of the grammar.
                n.reset();
            }
            else
            {
                // Otherwise, it's an actual else block, so we fold up the node
                // it contains.
                n = std::move(n->children.front());
            }
        }

    };
}}

#endif /* RHEA_TRANSFORM_ELSE_HPP */