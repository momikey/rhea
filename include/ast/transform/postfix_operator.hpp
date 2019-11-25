#ifndef RHEA_TRANSFORM_POSTFIX_HPP
#define RHEA_TRANSFORM_POSTFIX_HPP

#include <memory>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

#include "../../grammar.hpp"

namespace rhea { namespace ast {
    using namespace tao::pegtl;

    /*
     * Postfix operator transformer. This rearranges the parse tree
     * for Rhea's postfix operators: subscript, member access, and
     * function call. These are defined in a different manner from
     * the prefix unary operators, because PEG can't handle left
     * recursion. That complicates the logic, though.
     */

    struct postfix_rearrange : parse_tree::apply<postfix_rearrange>
    {
        template< typename NodeType, typename... States >
        static void transform( std::unique_ptr<NodeType>& n, States&&... st )
        {
            if (n->children.size() == 1)
            {
                // Fold a single child, as this is a bare operand.
                n = std::move(n->children.back());
            }
            else
            {
                // If there are multiple children, we want to bring up
                // the *last* child, as these are postfix operators.
                n->remove_content();

                auto& c = n->children;

                auto postfixop = std::move(c.back());
                c.pop_back();

                postfixop->children.emplace_back(std::move(n));
                n = std::move(postfixop);

                // This one has to be recursive. We use back() because
                // we're descending into *this* node (which is now a child
                // of the postfix expression).
                transform(n->children.back(), st...);
            }
            
        }
    };
}}

#endif /* RHEA_TRANSFORM_POSTFIX_HPP */