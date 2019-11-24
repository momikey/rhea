#ifndef RHEA_TRANSFORM_UNARY_HPP
#define RHEA_TRANSFORM_UNARY_HPP


#include <memory>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

#include "../../grammar.hpp"

namespace rhea { namespace ast {
    using namespace tao::pegtl;

    /*
     * Transformer for unary operators. These are a lot simpler,
     * because there's only one operand. Other than that, the logic
     * is the same as for binary operators.
     */

    struct unary_rearrange : parse_tree::apply<unary_rearrange>
    {
        template< typename NodeType, typename... States >
        static void transform( std::unique_ptr<NodeType>& n, States&&... st )
        {
            if (n->children.size() == 1)
            {
                // If this node has a single child, replace it with that child.
                n = std::move(n->children.back());
            }
            else
            {
                // Otherwise, bring up the operator, and make the operand
                // its child.
                n->remove_content();

                auto& c = n->children;

                auto operand = std::move(c.back());
                c.pop_back();

                auto unaryop = std::move(c.back());
                c.pop_back();

                unaryop->children.emplace_back(std::move(operand));
                n = std::move(unaryop);
            }
            
        }
    };

}}

#endif /* RHEA_TRANSFORM_UNARY_HPP */