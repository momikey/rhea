#ifndef RHEA_DEBUG_PARSE_TREE_HPP
#define RHEA_DEBUG_PARSE_TREE_HPP

#include <string>
#include <iostream>
#include <type_traits>
#include <memory>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

#include "../grammar/module.hpp"
#include "../grammar/statement.hpp"
#include "../ast/selector.hpp"
#include "../ast/parse_tree_node.hpp"

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
            internal::print_node(os, root, 0u);
        }
    }

    void print_parse_tree(std::ostream& os, std::string& input)
    {
        tao::pegtl::string_input<> in(input, "debug");
        auto root = pt::parse< 
            tao::pegtl::sor<
                rhea::grammar::program_definition,
                rhea::grammar::stmt_or_block
            >,
            rhea::ast::parser_node,
            rhea::ast::tree_selector
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

    auto input_from_string(std::string& input)
    {
        return std::make_unique<tao::pegtl::string_input<>>(input, "debug");
    }

    template <typename Node = tao::pegtl::parse_tree::node>
    std::unique_ptr<Node> parse(tao::pegtl::string_input<>& in)
    {
        // tao::pegtl::string_input<> in(input, "debug");

        try
        {
            auto root = pt::parse< 
                tao::pegtl::sor<
                    rhea::grammar::program_definition,
                    rhea::grammar::stmt_or_block
                >,
                Node,
                rhea::ast::tree_selector
            > (in);

            return std::move(root);
        }
        catch (tao::pegtl::parse_error& error)
        {
            const auto p = error.positions.front();
            std::cerr
                << error.what() << '\n'
                << in.line_at(p) << '\n'
                << std::string(p.byte_in_line, ' ') << "^\n";
        }

        return nullptr;
    }

}}

#endif /* RHEA_DEBUG_PARSE_TREE_HPP */