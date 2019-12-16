#include "ast/internal/builder.hpp"

#include <map>
#include <memory>
#include <cassert>

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

        const std::map<std::string, BasicType> integer_suffixes {
            { "_b", BasicType::Byte },
            { "_l", BasicType::Long },
            { "_u", BasicType::UnsignedInteger },
            { "_ub", BasicType::UnsignedByte },
            { "_ul", BasicType::UnsignedLong }
        };
        
        // Builder for identifiers, for when a more general expression can't be used.
        std::unique_ptr<AnyIdentifier> create_identifier_node(parser_node* node)
        {
            std::unique_ptr<AnyIdentifier> ident;

            // Simple identifiers
            if (node->is<gr::identifier>())
            {
                ident = std::make_unique<Identifier>(node->string());
            }

            else
            {
                throw unimplemented_type(node->name());
            }

            assert(ident != nullptr);
            ident->position = node->begin();
            return ident;
        }

        // Builder helper for binary operators
        expression_ptr create_binop_node(parser_node* node, BinaryOperators op)
        {
            return make_expression<BinaryOp>(
                op,
                create_expression_node(node->children.at(0).get()),
                create_expression_node(node->children.at(1).get())
            );
        }

        // Builder for expressions.
        expression_ptr create_expression_node(parser_node* node)
        {
            expression_ptr expr;

            // Floating-point literals
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

            // Integer literals
            else if (node->is<gr::integer_literal>())
            {
                std::string lit { node->children.front()->string() };

                if (!node->children.back()->is<gr::integer_literal_suffix>())
                {
                    // No suffix means use a default signed integer. This is 32-bit
                    // at present, though we may want to make it 64-bit later on.
                    int32_t i = std::stoi(lit);
                    return make_expression<Integer>(i);
                }
                else
                {
                    auto inttype = integer_suffixes.at(node->children.back()->string());
                    switch (inttype)
                    {
                        case BasicType::Byte:
                        {
                            int8_t i = std::stoi(lit);
                            return make_expression<Byte>(i);
                        }
                        case BasicType::Long:
                        {
                            int64_t i = std::stoll(lit);
                            return make_expression<Long>(i);
                        }
                        case BasicType::UnsignedInteger:
                        {
                            uint32_t i = std::stoi(lit);
                            return make_expression<UnsignedInteger>(i);
                        }
                        case BasicType::UnsignedByte:
                        {
                            uint8_t i = std::stoi(lit);
                            return make_expression<UnsignedByte>(i);
                        }
                        case BasicType::UnsignedLong:
                        {
                            uint64_t i = std::stoll(lit);
                            return make_expression<UnsignedLong>(i);
                        }
                        default:
                            throw unimplemented_type(node->name());
                    }
                }
            }

            // Binary operators: all of these delegate to the helper defined above.
            else if (node->is<gr::add_operator>())
            {
                return create_binop_node(node, BinaryOperators::Add);
            }
            else if (node->is<gr::subtract_operator>())
            {
                return create_binop_node(node, BinaryOperators::Subtract);
            }
            else if (node->is<gr::multiply_operator>())
            {
                return create_binop_node(node, BinaryOperators::Multiply);
            }
            else if (node->is<gr::divide_operator>())
            {
                return create_binop_node(node, BinaryOperators::Divide);
            }
            else if (node->is<gr::modulus_operator>())
            {
                return create_binop_node(node, BinaryOperators::Modulus);
            }
            else if (node->is<gr::exponent_operator>())
            {
                return create_binop_node(node, BinaryOperators::Exponent);
            }
            else if (node->is<gr::left_shift_operator>())
            {
                return create_binop_node(node, BinaryOperators::LeftShift);
            }
            else if (node->is<gr::right_shift_operator>())
            {
                return create_binop_node(node, BinaryOperators::RightShift);
            }
            else if (node->is<gr::equals_operator>())
            {
                return create_binop_node(node, BinaryOperators::Equals);
            }
            else if (node->is<gr::not_equal_operator>())
            {
                return create_binop_node(node, BinaryOperators::NotEqual);
            }
            else if (node->is<gr::less_than_operator>())
            {
                return create_binop_node(node, BinaryOperators::LessThan);
            }
            else if (node->is<gr::greater_than_operator>())
            {
                return create_binop_node(node, BinaryOperators::GreaterThan);
            }
            else if (node->is<gr::less_equal_operator>())
            {
                return create_binop_node(node, BinaryOperators::LessThanOrEqual);
            }
            else if (node->is<gr::greater_equal_operator>())
            {
                return create_binop_node(node, BinaryOperators::GreaterThanOrEqual);
            }
            else if (node->is<gr::bitand_operator>())
            {
                return create_binop_node(node, BinaryOperators::BitAnd);
            }
            else if (node->is<gr::bitor_operator>())
            {
                return create_binop_node(node, BinaryOperators::BitOr);
            }
            else if (node->is<gr::bitxor_operator>())
            {
                return create_binop_node(node, BinaryOperators::BitXor);
            }
            else if (node->is<gr::kw_and>())
            {
                return create_binop_node(node, BinaryOperators::BooleanAnd);
            }
            else if (node->is<gr::kw_or>())
            {
                return create_binop_node(node, BinaryOperators::BooleanOr);
            }


            else
            {
                throw unimplemented_type(node->name());
            }

            assert(expr != nullptr);
            expr->position = node->begin();
            return expr;
        }

        // Builder for statements.
        statement_ptr create_statement_node(parser_node* node)
        {
            statement_ptr stmt;

            // Bare expressions used in statement context: `foo();`
            if (node->is<gr::bare_expression>())
            {
                stmt = make_statement<BareExpression>(std::move(
                    create_expression_node(node->children.at(0).get())
                ));
            }

            // Constant declaration: `const bar = 42;`
            else if (node->is<gr::constant_declaration>())
            {
                stmt = make_statement<Constant>(
                    std::move(create_identifier_node(node->children.at(0).get())),
                    std::move(create_expression_node(node->children.at(1).get()))
                );
            }

            else
            {
                throw unimplemented_type(node->name());
            }

            // We should have a non-null statement node by now, because every rule
            // not handled should throw.
            assert(stmt != nullptr);
            stmt->position = node->begin();
            return stmt;
        }

    }
}}