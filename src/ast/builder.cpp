#include "ast/internal/builder.hpp"

#include <map>
#include <vector>
#include <algorithm>
#include <memory>
#include <cassert>

namespace rhea { namespace ast {
    using types::BasicType;
    
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

        // Builder for typenames, because these can be complex.
        std::unique_ptr<Typename> create_typename_node(parser_node* node)
        {
            std::unique_ptr<Typename> tname;

            // Simple identifiers
            if (node->is<gr::identifier>())
            {
                tname = std::make_unique<Typename>(std::move(create_identifier_node(node)));
            }

            else
            {
                throw unimplemented_type(node->name());
            }

            assert(tname != nullptr);
            tname->position = node->begin();
            return tname;
        }

        // Builder helper for name-type pairs, which are common throughout the AST.
        std::unique_ptr<TypePair> create_typepair_node(parser_node* node)
        {
            return std::make_unique<TypePair>(
                node->children.at(0)->string(),
                std::move(create_typename_node(node->children.at(1).get()))
            );
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

        // Builder helper for unary operators
        expression_ptr create_unary_node(parser_node* node, UnaryOperators op)
        {
            return make_expression<UnaryOp>(
                op,
                create_expression_node(node->children.at(0).get())
            );
        }

        // Builder helper for named arguments
        std::unique_ptr<NamedArgument> create_named_argument(parser_node* node)
        {
            return std::make_unique<NamedArgument>(
                node->children.at(0)->string(),
                create_expression_node(node->children.at(1).get())
            );
        }

        // Builder helper for function definitions.
        // Note that this takes the type so we don't have to scan through them again.
        statement_ptr create_function_definition(parser_node* node, FunctionType type)
        {
            std::string name { "" };
            std::unique_ptr<TypePair> generic_type = nullptr;
            std::unique_ptr<Typename> return_type = nullptr;
            std::unique_ptr<Arguments> arguments_list = nullptr;
            child_vector<Condition> conditions;
            statement_ptr body = nullptr;

            // I don't like the idea of a for-switch, which is basically what this is.
            // Alas, the way functions are defined in the grammar doesn't really leave us
            // with many other options. If there's a better way, I'd like to know.
            for (auto& c : node->children)
            {
                // Function names are simple strings.
                if (c->is<gr::function_name>())
                {
                    name = c->string();
                }
                // Gneeric types are handled specially.
                else if (c->is<gr::generic_function_type>())
                {
                    // We're not ready for this just yet.
                    throw unimplemented_type(node->name());
                }
                // Return types are simple typenames.
                else if (c->is<gr::return_type>())
                {
                    return_type = create_typename_node(c->children.front().get());
                }
                // Argument lists are in their own node.
                else if (c->is<gr::arguments_list>())
                {
                    if (c->children.front()->is<gr::wildcard_argument>())
                    {
                        // We'll get to this later.
                        throw unimplemented_type(node->name());
                    }
                    else
                    {
                        std::vector<std::unique_ptr<TypePair>> args;
                        auto& ch = c->children;
                        std::for_each(ch.begin(), ch.end(), 
                            [&](std::unique_ptr<parser_node>& el)
                            {
                                if (el->is<gr::type_match>())
                                {
                                    args.emplace_back(std::move(create_typepair_node(el.get())));                                    
                                }
                                else
                                {
                                    throw unimplemented_type(el->name());
                                }
                            }
                        );

                        arguments_list = std::make_unique<Arguments>(args);
                    }
                }
                // Conditions are in a pointer vector, with the usual logic.
                else if (c->is<gr::fn_with_block>())
                {
                    // We'll do it later.
                    throw unimplemented_type(node->name());
                }
                // Anything else should be considered the function body.
                else
                {
                    body = create_statement_node(c.get());
                }
            }

            return make_statement<Def>(
                type,
                name,
                std::move(generic_type),
                std::move(return_type),
                std::move(arguments_list),
                conditions,
                std::move(body)
            );
        }

        // Helper to map node types to the operator enum for compound assignments.
        AssignOperator assignment_operator_type(parser_node* node)
        {
            if (node->is<gr::add_operator>())
                return AssignOperator::Add;
            else if (node->is<gr::subtract_operator>())
                return AssignOperator::Subtract;
            else if (node->is<gr::multiply_operator>())
                return AssignOperator::Multiply;
            else if (node->is<gr::divide_operator>())
                return AssignOperator::Divide;
            else if (node->is<gr::modulus_operator>())
                return AssignOperator::Modulus;
            else if (node->is<gr::exponent_operator>())
                return AssignOperator::Exponent;
            else if (node->is<gr::left_shift_operator>())
                return AssignOperator::LeftShift;
            else if (node->is<gr::right_shift_operator>())
                return AssignOperator::RightShift;
            else if (node->is<gr::bitand_operator>())
                return AssignOperator::BitAnd;
            else if (node->is<gr::bitor_operator>())
                return AssignOperator::BitOr;
            else if (node->is<gr::bitxor_operator>())
                return AssignOperator::BitXor;
            else
                throw unimplemented_type(node->name());
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
                    expr = make_expression<Float>(std::stof(lit));
                }
                else
                {
                    expr = make_expression<Double>(std::stod(lit));
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
                    expr = make_expression<Integer>(i);
                }
                else
                {
                    auto inttype = integer_suffixes.at(node->children.back()->string());
                    switch (inttype)
                    {
                        case BasicType::Byte:
                        {
                            int8_t i = std::stoi(lit);
                            expr = make_expression<Byte>(i);
                        }
                        case BasicType::Long:
                        {
                            int64_t i = std::stoll(lit);
                            expr = make_expression<Long>(i);
                        }
                        case BasicType::UnsignedInteger:
                        {
                            uint32_t i = std::stoi(lit);
                            expr = make_expression<UnsignedInteger>(i);
                        }
                        case BasicType::UnsignedByte:
                        {
                            uint8_t i = std::stoi(lit);
                            expr = make_expression<UnsignedByte>(i);
                        }
                        case BasicType::UnsignedLong:
                        {
                            uint64_t i = std::stoll(lit);
                            expr = make_expression<UnsignedLong>(i);
                        }
                        default:
                            throw unimplemented_type(node->name());
                    }
                }
            }

            // Hexadecimal literals: these can be either 32-bit or 64-bit;
            // 64-bit is autodetected if the literal contains more than 8
            // hex digits. Note that hex literals are unsigned by default.
            else if (node->is<gr::hex_literal>())
            {
                // Check for size up to 10, because of the leading "0x"
                if (node->string().length() <= 10)
                {
                    expr = make_expression<UnsignedInteger>(std::stoi(node->string(), nullptr, 16));
                }
                else
                {
                    expr = make_expression<UnsignedLong>(std::stoll(node->string(), nullptr, 16));
                }
            }

            // Boolean literals
            else if (node->is<gr::boolean_literal>())
            {
                expr = std::make_unique<Boolean>(node->string() == "true");
            }

            // "Nothing" type literals
            else if (node->is<gr::nothing_literal>())
            {
                expr = std::make_unique<Nothing>();
            }

            // String literals: these are difficult, because we have to "unescape" them
            // before passing them to codegen. I wrestled with the decision on where to
            // do that, but I've decided to pass the buck here. Let the AST node itself
            // be responsible for that when the time comes. That also helps serialization,
            // since we don't have to go back and forth as much.
            else if (node->is<gr::string_literal>())
            {
                // String literal parse nodes have a single child containing the string
                // itself. These are wrapped in a "quote" node, because Rhea strings can
                // be single or double quoted. But we don't care about that by this point,
                // so they're normalized to double quotes in the AST, and we just ignore
                // whatever the user initially chose.
                expr = make_expression<String>(node->children.front()->string());
            }

            // Simple identifiers
            else if (node->is<gr::identifier>())
            {
                expr = std::make_unique<Identifier>(node->string());
            }

            // Symbols
            else if (node->is<gr::symbol_name>())
            {
                // Symbol parse nodes always have a single child with the symbol name
                // as its content.
                expr = std::make_unique<Symbol>(node->children.at(0)->string());
            }

            // Binary operators: all of these delegate to the helper defined above.
            else if (node->is<gr::add_operator>())
            {
                expr = create_binop_node(node, BinaryOperators::Add);
            }
            else if (node->is<gr::subtract_operator>())
            {
                expr = create_binop_node(node, BinaryOperators::Subtract);
            }
            else if (node->is<gr::multiply_operator>())
            {
                expr = create_binop_node(node, BinaryOperators::Multiply);
            }
            else if (node->is<gr::divide_operator>())
            {
                expr = create_binop_node(node, BinaryOperators::Divide);
            }
            else if (node->is<gr::modulus_operator>())
            {
                expr = create_binop_node(node, BinaryOperators::Modulus);
            }
            else if (node->is<gr::exponent_operator>())
            {
                expr = create_binop_node(node, BinaryOperators::Exponent);
            }
            else if (node->is<gr::left_shift_operator>())
            {
                expr = create_binop_node(node, BinaryOperators::LeftShift);
            }
            else if (node->is<gr::right_shift_operator>())
            {
                expr = create_binop_node(node, BinaryOperators::RightShift);
            }
            else if (node->is<gr::equals_operator>())
            {
                expr = create_binop_node(node, BinaryOperators::Equals);
            }
            else if (node->is<gr::not_equal_operator>())
            {
                expr = create_binop_node(node, BinaryOperators::NotEqual);
            }
            else if (node->is<gr::less_than_operator>())
            {
                expr = create_binop_node(node, BinaryOperators::LessThan);
            }
            else if (node->is<gr::greater_than_operator>())
            {
                expr = create_binop_node(node, BinaryOperators::GreaterThan);
            }
            else if (node->is<gr::less_equal_operator>())
            {
                expr = create_binop_node(node, BinaryOperators::LessThanOrEqual);
            }
            else if (node->is<gr::greater_equal_operator>())
            {
                expr = create_binop_node(node, BinaryOperators::GreaterThanOrEqual);
            }
            else if (node->is<gr::bitand_operator>())
            {
                expr = create_binop_node(node, BinaryOperators::BitAnd);
            }
            else if (node->is<gr::bitor_operator>())
            {
                expr = create_binop_node(node, BinaryOperators::BitOr);
            }
            else if (node->is<gr::bitxor_operator>())
            {
                expr = create_binop_node(node, BinaryOperators::BitXor);
            }
            else if (node->is<gr::kw_and>())
            {
                expr = create_binop_node(node, BinaryOperators::BooleanAnd);
            }
            else if (node->is<gr::kw_or>())
            {
                expr = create_binop_node(node, BinaryOperators::BooleanOr);
            }

            // Unary operators: all of these delegate to the helper above.
            else if (node->is<gr::coerce_operator>())
            {
                expr = create_unary_node(node, UnaryOperators::Coerce);
            }
            else if (node->is<gr::bitnot_operator>())
            {
                expr = create_unary_node(node, UnaryOperators::BitNot);
            }
            else if (node->is<gr::dereference_operator>())
            {
                expr = create_unary_node(node, UnaryOperators::Dereference);
            }
            else if (node->is<gr::unary_plus_operator>())
            {
                expr = create_unary_node(node, UnaryOperators::Plus);
            }
            else if (node->is<gr::unary_minus_operator>())
            {
                expr = create_unary_node(node, UnaryOperators::Minus);
            }
            else if (node->is<gr::kw_not>())
            {
                expr = create_unary_node(node, UnaryOperators::BooleanNot);
            }
            else if (node->is<gr::kw_ref>())
            {
                expr = create_unary_node(node, UnaryOperators::Ref);
            }
            else if (node->is<gr::kw_ptr>())
            {
                expr = create_unary_node(node, UnaryOperators::Ptr);
            }

            // Array expressions: `[1,2,3]`
            else if (node->is<gr::array_expression>())
            {
                std::vector<expression_ptr> exs;
                auto& ch = node->children;
                std::for_each(ch.begin(), ch.end(), 
                    [&](std::unique_ptr<parser_node>& el)
                    { exs.emplace_back(std::move(create_expression_node(el.get()))); }
                );

                expr = make_expression<Array>(exs);
            }
            // List expressions: `(1,2,3)`
            else if (node->is<gr::list_expression>())
            {
                std::vector<expression_ptr> exs;
                auto& ch = node->children;
                std::for_each(ch.begin(), ch.end(), 
                    [&](std::unique_ptr<parser_node>& el)
                    { exs.emplace_back(std::move(create_expression_node(el.get()))); }
                );

                expr = make_expression<List>(exs);
            }
            // Tuple expressions: `{1,2,3}`
            else if (node->is<gr::tuple_expression>())
            {
                std::vector<expression_ptr> exs;
                auto& ch = node->children;
                std::for_each(ch.begin(), ch.end(), 
                    [&](std::unique_ptr<parser_node>& el)
                    { exs.emplace_back(std::move(create_expression_node(el.get()))); }
                );

                expr = make_expression<Tuple>(exs);
            }
            // Symbol list expressions: `@{a,b,c}`
            else if (node->is<gr::symbol_list_expression>())
            {
                std::vector<std::string> syms;
                auto& ch = node->children;
                std::for_each(ch.begin(), ch.end(), 
                    [&](std::unique_ptr<parser_node>& el)
                    { syms.push_back(el->string()); }
                );

                expr = make_expression<SymbolList>(syms);
            }

            // Member access expressions: `x.y`
            else if (node->is<gr::member_expr>())
            {
                expr = make_expression<Member>(
                    std::move(std::make_unique<Identifier>(node->children.at(0)->string())),
                    std::move(create_expression_node(node->children.at(1).get()))
                );
            }

            // Subscript expressions: `x[y]`
            // Note that the order is swapped. This is intentional.
            else if (node->is<gr::subscript_expr>())
            {
                expr = make_expression<Subscript>(
                    std::move(create_expression_node(node->children.at(1).get())),
                    std::move(create_expression_node(node->children.at(0).get()))
                );
            }

            // Ternary expressions: `if a then b else c`
            else if (node->is<gr::ternary_op>())
            {
                expr = make_expression<TernaryOp>(
                    std::move(create_expression_node(node->children.at(0).get())),
                    std::move(create_expression_node(node->children.at(1).get())),
                    std::move(create_expression_node(node->children.at(2).get()))
                );
            }

            // Function call expressions
            else if (node->is<gr::function_call_expr>())
            {
                // Function calls have 3 possible parse trees. In all cases, the called
                // expression is the 2nd child, while the 1st holds the match for the
                // function's arguments, which can be one of 3 different parse rules.
                auto fn = create_expression_node(node->children.at(1).get());

                auto& args = node->children.at(0);

                // Empty argument list: `f()`
                if (args->is<gr::empty_argument_list>())
                {
                    expr = make_expression<Call>(std::move(fn));
                }
                // Positional argument list: `f(1,2)`
                else if (args->is<gr::unnamed_argument_list>())
                {
                    std::vector<expression_ptr> exs;
                    auto& ch = args->children;
                    std::for_each(ch.begin(), ch.end(), 
                        [&](std::unique_ptr<parser_node>& el)
                        { exs.emplace_back(std::move(create_expression_node(el.get()))); }
                    );

                    expr = make_expression<Call>(std::move(fn), exs);
                }
                // Named argument list: `f(a: 1, b: 2)`
                else if (args->is<gr::named_argument_list>())
                {
                    std::vector<std::unique_ptr<NamedArgument>> exs;
                    auto& ch = args->children;
                    std::for_each(ch.begin(), ch.end(), 
                        [&](std::unique_ptr<parser_node>& el)
                        { exs.emplace_back(std::move(create_named_argument(el.get()))); }
                    );

                    expr = make_expression<Call>(std::move(fn), exs);
                }
                // Something else, which shouldn't happen unless someone's trying to
                // manually construct a parse tree, or there's a bug in the grammar.
                else
                {
                    throw unimplemented_type(node->name());
                }
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

            // Statement blocks: `{ foo(); bar(); }`
            else if (node->is<gr::statement_block>())
            {
                std::vector<statement_ptr> block_stmts;
                auto& ch = node->children;
                std::for_each(ch.begin(), ch.end(), 
                    [&](std::unique_ptr<parser_node>& el)
                    { block_stmts.emplace_back(std::move(create_statement_node(el.get()))); }
                );

                stmt = make_statement<Block>(block_stmts);
            }

            // Variable definition: `var x = y * z;`
            else if (node->is<gr::variable_declaration>())
            {
                stmt = make_statement<Variable>(
                    std::move(std::make_unique<Identifier>(node->children.at(0)->string())),
                    std::move(create_expression_node(node->children.at(1).get()))
                );
            }

            // Variable declaration: `var x as y;`
            else if (node->is<gr::declaration_as_type>())
            {
                stmt = make_statement<TypeDeclaration>(
                    std::move(std::make_unique<Identifier>(node->children.at(0)->string())),
                    std::move(create_typename_node(node->children.at(1).get()))
                );
            }

            // Constant definitino: `const bar = 42;`
            else if (node->is<gr::constant_declaration>())
            {
                stmt = make_statement<Constant>(
                    std::move(std::make_unique<Identifier>(node->children.at(0)->string())),
                    std::move(create_expression_node(node->children.at(1).get()))
                );
            }

            // Variable assignment: `foo = bar ** 2;`
            else if (node->is<gr::assignment>())
            {
                stmt = make_statement<Assign>(
                    std::move(create_expression_node(node->children.at(0).get())),
                    std::move(create_expression_node(node->children.at(1).get()))
                );
            }

            // Compound assignment: `i -= 1;`
            else if (node->is<gr::compound_assignment>())
            {
                stmt = make_statement<CompoundAssign>(
                    std::move(create_expression_node(node->children.at(0).get())),
                    assignment_operator_type(node->children.at(1).get()),
                    std::move(create_expression_node(node->children.at(2).get()))
                );
            }

            // Enum declaration: `type En = @{a,b,c};`
            else if (node->is<gr::enum_declaration>())
            {
                std::vector<std::string> syms;
                auto& ch = node->children.at(1)->children;
                std::for_each(ch.begin(), ch.end(), 
                    [&](std::unique_ptr<parser_node>& el)
                    { syms.push_back(el->string()); }
                );

                stmt = make_statement<Enum>(
                    std::move(std::make_unique<Identifier>(node->children.at(0)->string())),
                    std::move(std::make_unique<SymbolList>(syms))
                );
            }

            // Do statement: `do that;`
            // TODO: Should we do the bare identifier -> function call conversion here?
            else if (node->is<gr::do_statement>())
            {
                stmt = make_statement<Do>(
                    std::move(create_expression_node(node->children.at(0).get()))
                );
            }

            // If statement: `if (x > 0) { do this; } else { do that; }`
            else if (node->is<gr::if_statement>())
            {
                stmt = make_statement<If>(
                    std::move(create_expression_node(node->children.at(0).get())),
                    std::move(create_statement_node(node->children.at(1).get())),
                    node->children.size() > 2
                        ? std::move(create_statement_node(node->children.at(2).get()))
                        : nullptr
                );
            }

            // Unless statement: `unless (x == nothing) { do x; }`
            else if (node->is<gr::unless_statement>())
            {
                // An "unless" is just an "if" with an inverted condition and no else.
                // We model that by making an If AST node that has a null "then" part.
                stmt = make_statement<If>(
                    std::move(create_expression_node(node->children.at(0).get())),
                    nullptr,
                    std::move(create_statement_node(node->children.at(1).get()))
                );
            }

            // While statement: `while x < 10 { x += 1; }`
            else if (node->is<gr::while_statement>())
            {
                stmt = make_statement<While>(
                    std::move(create_expression_node(node->children.at(0).get())),
                    std::move(create_statement_node(node->children.at(1).get()))
                );
            }

            // For statement: `for i in range { do foo; }`
            else if (node->is<gr::for_statement>())
            {
                stmt = make_statement<For>(
                    node->children.at(0)->string(),
                    std::move(create_expression_node(node->children.at(1).get())),
                    std::move(create_statement_node(node->children.at(2).get()))
                );
            }

            // Break and continue statements are just empty nodes.
            else if (node->is<gr::kw_break>())
            {
                stmt = make_statement<Break>();
            }
            else if (node->is<gr::kw_continue>())
            {
                stmt = make_statement<Continue>();
            }

            // Function definitions, of various kinds. The inner logic is handled
            // in the `create_function_definition` helper above. Here, we only
            // store the type of the function being declared.
            else if (node->is<gr::basic_function_def>())
            {
                stmt = std::move(create_function_definition(node, FunctionType::Basic));
            }
            else if (node->is<gr::unchecked_function_def>())
            {
                stmt = std::move(create_function_definition(node, FunctionType::Unchecked));
            }
            else if (node->is<gr::predicate_function_def>())
            {
                stmt = std::move(create_function_definition(node, FunctionType::Predicate));
            }
            else if (node->is<gr::operator_function_def>())
            {
                stmt = std::move(create_function_definition(node, FunctionType::Operator));
            }

            // Return statement: `return false;`
            else if (node->is<gr::return_statement>())
            {
                stmt = make_statement<Return>(
                    std::move(create_expression_node(node->children.front().get()))
                );
            }

            // Extern declaration: `extern foo;`
            else if (node->is<gr::extern_declaration>())
            {
                stmt = make_statement<Extern>(node->children.front()->string());
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

        // Builder for top-level structures, i.e., modules and programs. This will be
        // the main entry point to the AST builder in real situations, unless someone
        // makes a REPL or something.
        std::unique_ptr<ASTNode> create_top_level_node(parser_node* node)
        {
            std::unique_ptr<ASTNode> ast_node;

            if (node->is<gr::program_definition>())
            {
                std::vector<statement_ptr> stmts;
                auto& ch = node->children;
                std::for_each(ch.begin(), ch.end(), 
                    [&](std::unique_ptr<parser_node>& el)
                    { stmts.emplace_back(std::move(create_statement_node(el.get()))); }
                );

                ast_node = std::make_unique<Program>(stmts);
            }

            else
            {
                throw unimplemented_type(node->name());
            }

            assert(ast_node != nullptr);
            ast_node->position = node->begin();
            return ast_node;
        }
    }
}}