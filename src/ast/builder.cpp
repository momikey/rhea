#include "ast/internal/builder.hpp"

namespace rhea { namespace ast {
    // Build a Rhea AST out of the PEGTL modified parse tree.
    std::unique_ptr<ASTNode> build_ast(parser_node* node)
    {
        // PRGTL's parse tree builder always creates a root node that does nothing.
        // Since our grammar guarantees that we only ever have a single root, we
        // don't really need that, and we can just build our AST from the PEGTL root's
        // only child.
        // TODO: We have to determine whether we're dealing with a module or program.
        // return create_program_node(node->children().back());
        throw unimplemented_type("Root node");
    }

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
        std::unique_ptr<AnyIdentifier> create_identifier_node(parser_node* node)
        {
            throw unimplemented_type(node->name());
        }

        // Builder for expressions.
        expression_ptr create_expression_node(parser_node* node)
        {
            if (node->is<gr::float_literal>())
            {
                std::string lit { node->children.front()->string() };

                // If the float literal suffix "_f" is present, create a float,
                // otherwise make it a double.
                if (node->children.back()->is<gr::float_literal_suffix>())
                {
                    return make_expression<Float>(std::stof(lit));
                }
                else
                {
                    return make_expression<Double>(std::stod(lit));
                }
            }

            else
            {
                throw unimplemented_type(node->name());
            }
        }

        // Builder for statements.
        statement_ptr create_statement_node(parser_node* node)
        {
            if (node->is<gr::bare_expression>())
            {
                auto stmt = make_statement<BareExpression>(std::move(
                    create_expression_node(node->children.at(0).get())
                ));
                stmt->position = node->begin();
                return stmt;
            }

            else
            {
                throw unimplemented_type(node->name());
            }
        }

    }
}}