#ifndef RHEA_DEBUG_PARSE_TREE_HPP
#define RHEA_DEBUG_PARSE_TREE_HPP

#include <string>
#include <iostream>
#include <type_traits>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

#include "../grammar/tokens.hpp"
#include "../grammar/operators.hpp"

namespace rhea { namespace debug {
    namespace pt = tao::pegtl::parse_tree;

    template <typename Rule>
    struct selector : std::true_type {};

    namespace internal {
        template <typename Node>
        void print_node(std::ostream& os, const Node& n, int level)
        {
            if (n->has_content())
            {
                std::string space(level, ' ' );
                os << level << space << n->name() << '\t' << n->string() << '\n';
            }

            if (!n->children.empty())
            {
                for (auto&& child : n->children)
                {
                    print_node(os, child, level+1);
                }
            }
        }
    }

    void print_parse_tree(std::ostream& os, std::string& input)
    {
        tao::pegtl::string_input<> in(input, "debug");
        auto root = pt::parse< rhea::grammar::expression, selector > (in);

        internal::print_node(os, root, 0u);
    }


}}

#endif /* RHEA_DEBUG_PARSE_TREE_HPP */