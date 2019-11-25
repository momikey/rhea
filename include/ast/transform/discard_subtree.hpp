#ifndef RHEA_TRANSFORM_DISCARD_HPP
#define RHEA_TRANSFORM_DISCARD_HPP

#include <memory>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

#include "../../grammar.hpp"

namespace rhea { namespace ast {
    using namespace tao::pegtl;

    /* This transformer discards a node and all its children from
     * the parse tree. We need this in some cases where we use
     * lookahead, because PEGTL doesn't disable node creation during
     * an `at` rule.
     */

    struct discard_subtree : parse_tree::apply<discard_subtree>
    {
        template< typename NodeType, typename... States >
        static void transform( std::unique_ptr<NodeType>& n, States&&... st )
        {
            n.reset();
        }
    };
}}

#endif /* RHEA_TRANSFORM_DISCARD_HPP */