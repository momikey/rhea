#ifndef RHEA_AST_BUILDER_INTERNAL_HPP
#define RHEA_AST_BUILDER_INTERNAL_HPP

#include "../../grammar.hpp"
#include "../nodes.hpp"
#include "../parse_tree_node.hpp"

/*
 * Internal detail for the AST builder.
 */
namespace rhea { namespace ast {
    namespace internal {
        namespace gr = rhea::grammar;

        /*
         * Except for rewriting a good chunk of PEGTL's parse tree builder,
         * I don't see any obvious way to do this besides a big type switch.
         * At the very least, we can break it down into more manageable parts.
         * Thus, here we define a few node creation functions, one for each
         * of the "main" AST node types. Unfortunately, there's not too much
         * we can do with templates here, because the node constructors take
         * different arguments, and we have to decide at run time which one
         * to use.
         */
        
        // Builder for identifiers, for when a more general expression can't be used.
        template <typename Node>
        std::unique_ptr<AnyIdentifier> create_identifier_node(Node* node)
        {

        }

        // Builder for expressions.
        template <typename Node>
        expression_ptr create_expression_node(Node* node)
        {

        }

        // Builder for statements.
        template <typename Node>
        statement_ptr create_statement_node(Node* node)
        {

        }

    }
}}

#endif /* RHEA_AST_BUILDER_INTERNAL_HPP */