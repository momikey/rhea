#ifndef RHEA_TRANSFORM_TERNARY_HPP
#define RHEA_TRANSFORM_TERNARY_HPP

#include <memory>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

#include "../../grammar.hpp"

namespace rhea { namespace ast {
    using namespace tao::pegtl;

    /*
     * Ternary operator transformer. This one's a little tricky, because
     * we have 3 keywords involved. But what we can do is not generate
     * parse nodes for those, and thus ignore them. That effectively
     * reduces the problem to PEGTL's built-in fold_one transformer, but
     * we might need further processing.
     */

    struct ternary_transform : parse_tree::apply<ternary_transform>
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
                // Remove this node's content.
                n->remove_content();

                // Since we're only generating nodes for the conditional
                // and the two branches, that's really all we need.
            }
        }
    };
}}

#endif /* RHEA_TRANSFORM_TERNARY_HPP */