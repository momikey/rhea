#ifndef RHEA_DEBUG_PARSE_TREE_HPP
#define RHEA_DEBUG_PARSE_TREE_HPP

#include <string>
#include <iostream>
#include <type_traits>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

#include "../grammar/tokens.hpp"
#include "../grammar/expression.hpp"
#include "../grammar/statement.hpp"
#include "../ast/selector.hpp"

namespace rhea { namespace debug {
    namespace pt = tao::pegtl::parse_tree;

    // TODO: Make selectors for all the right nodes
    // (This will be in a separate AST namespace.)
    template <typename Rule>
    struct selector : std::true_type {};

    namespace internal {
        template <typename Node>
        void print_node(std::ostream& os, const Node& n, int level)
        {
            std::string space(level, ' ' );

            if (n->has_content())
            {
                os << level << space << n->name() << '\t' << n->string() << '\n';
            }
            else if (!n->is_root())
            {
                os << level << space << n->name() << '\n';
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

    template <typename Node>
    void print_partial_tree(std::ostream& os, Node* root)
    {
        if (root)
        {
            internal::print_node(os, root, 0);
        }
    }

    void print_parse_tree(std::ostream& os, std::string& input)
    {
        tao::pegtl::string_input<> in(input, "debug");
        auto root = pt::parse< 
            tao::pegtl::sor<
                rhea::grammar::program_definition,
                rhea::grammar::stmt_or_block,
                rhea::grammar::expression
            >,
            rhea::ast::tree_selector
            // selector
        > (in);

        if (root)
        {
            internal::print_node(os, root, 0u);
        }
        else
        {
            std::cerr << "Parse error\n";
        }
    }

}}

#endif /* RHEA_DEBUG_PARSE_TREE_HPP */