#include "inference/visitor.hpp"
#include "inference/engine.hpp"

namespace rhea { namespace inference {
    using namespace rhea::ast;
    using namespace rhea::types;
    using util::any;

    using type_vector = std::vector<TypeInfo>;

    /*
     * Note that there is no need for a return value in any of these.
     * 
     * Also, the inference engine's type map requires us to store pointers to the base
     * AST node class. As this visitor is (or *should be*) the only way those pointers
     * are created, we can guarantee that they point to the correct derived class, and
     * thus we can use static_cast instead of the much slower dynamic_cast.
     */

    any InferenceVisitor::visit(Boolean* n)
    {
        engine->inferred_types[n] =
            InferredType { [](TypeEngine* e, ASTNode* node) { return SimpleType(BasicType::Boolean, false); } };
        return {};
    }

    any InferenceVisitor::visit(Integer* n)
    {
        engine->inferred_types[n] =
            InferredType { [](TypeEngine* e, ASTNode* node) { return SimpleType(BasicType::Integer, true, true); } };
        return {};
    }

    any InferenceVisitor::visit(Byte* n)
    {
        engine->inferred_types[n] =
            InferredType { [](TypeEngine* e, ASTNode* node) { return SimpleType(BasicType::Byte, true, true); } };
        return {};
    }

    any InferenceVisitor::visit(Long* n)
    {
        engine->inferred_types[n] =
            InferredType { [](TypeEngine* e, ASTNode* node) { return SimpleType(BasicType::Long, true, true); } };
        return {};
    }

    any InferenceVisitor::visit(UnsignedInteger* n)
    {
        engine->inferred_types[n] =
            InferredType { [](TypeEngine* e, ASTNode* node) { return SimpleType(BasicType::UnsignedInteger, true, true); } };
        return {};
    }

    any InferenceVisitor::visit(UnsignedByte* n)
    {
        engine->inferred_types[n] =
            InferredType { [](TypeEngine* e, ASTNode* node) { return SimpleType(BasicType::UnsignedByte, true, true); } };
        return {};
    }

    any InferenceVisitor::visit(UnsignedLong* n)
    {
        engine->inferred_types[n] =
            InferredType { [](TypeEngine* e, ASTNode* node) { return SimpleType(BasicType::UnsignedLong, true, true); } };
        return {};
    }

    any InferenceVisitor::visit(Float* n)
    {
        engine->inferred_types[n] =
            InferredType { [](TypeEngine* e, ASTNode* node) { return SimpleType(BasicType::Float, true, false); } };
        return {};
    }

    any InferenceVisitor::visit(Double* n)
    {
        engine->inferred_types[n] =
            InferredType { [](TypeEngine* e, ASTNode* node) { return SimpleType(BasicType::Double, true, false); } };
        return {};
    }

    any InferenceVisitor::visit(Symbol* n)
    {
        engine->inferred_types[n] =
            InferredType { [](TypeEngine* e, ASTNode* node) { return SimpleType(BasicType::Symbol, false); } };
        return {};
    }

    any InferenceVisitor::visit(Nothing* n)
    {
        engine->inferred_types[n] =
            InferredType { [](TypeEngine* e, ASTNode* node) { return NothingType(); } };
        return {};
    }

    any InferenceVisitor::visit(BinaryOp* n)
    {
        n->left->visit(this);
        n->right->visit(this);
        
        engine->inferred_types[n] =
            InferredType {
                [](TypeEngine* e, ASTNode* node)
                {
                    auto derived = static_cast<BinaryOp*>(node);
                    auto lhs = e->inferred_types[derived->left.get()]();
                    auto rhs = e->inferred_types[derived->right.get()]();

                    if (is_boolean_op(derived->op))
                    {
                        return TypeInfo {SimpleType(BasicType::Boolean, false)};
                    }
                    else if (lhs == rhs)
                    {
                        return lhs;
                    }
                    else
                    {
                        return TypeInfo {UnknownType()};
                    }
                },
                engine, n
            };
        return {};
    }

    any InferenceVisitor::visit(UnaryOp* n)
    {
        n->operand->visit(this);

        engine->inferred_types[n] =
            InferredType {
                [](TypeEngine* e, ASTNode* node)
                {
                    auto derived = static_cast<UnaryOp*>(node);
                    
                    // Different unary operators have different type implications.
                    switch (derived->op)
                    {
                        case UnaryOperators::Coerce:
                        case UnaryOperators::Dereference:
                        case UnaryOperators::Ref:
                        case UnaryOperators::Ptr:
                            // TODO: Implement these
                            return TypeInfo {UnknownType()};
                        default:
                            // All others keep the type of their operand.
                            return e->inferred_types[derived->operand.get()]();
                    }
                },
                engine, n
            };
        return {};
    }

    any InferenceVisitor::visit(TernaryOp* n)
    {
        n->condition->visit(this);
        n->true_branch->visit(this);
        n->false_branch->visit(this);
        
        engine->inferred_types[n] =
            InferredType {
                [](TypeEngine* e, ASTNode* node)
                {
                    auto derived = static_cast<TernaryOp*>(node);
                    auto tb = e->inferred_types[derived->true_branch.get()]();
                    auto fb = e->inferred_types[derived->false_branch.get()]();

                    if (tb == fb)
                    {
                        return tb;
                    }
                    else
                    {
                        return TypeInfo {UnknownType()};
                    }
                },
                engine, n
            };
        return {};
    }

    any InferenceVisitor::visit(Typename* n)
    {
        engine->inferred_types[n] =
            InferredType {
                [](TypeEngine* e, ASTNode* node)
                {
                    auto derived = static_cast<Typename*>(node);

                    return e->mapper.get_type_for(derived->canonical_name());
                },
                engine, n
            };
        return {};
    }

    any InferenceVisitor::visit(Variant* n)
    {
        for (auto&& ch : n->children)
        {
            ch->visit(this);
        }
        
        engine->inferred_types[n] =
            InferredType {
                [](TypeEngine* e, ASTNode* node)
                {
                    auto derived = static_cast<Variant*>(node);

                    return e->mapper.get_type_for(derived->canonical_name());
                },
                engine, n
            };
        return {};
    }

    any InferenceVisitor::visit(Optional* n)
    {
        n->type->visit(this);

        engine->inferred_types[n] =
            InferredType {
                [](TypeEngine* e, ASTNode* node)
                {
                    auto derived = static_cast<Optional*>(node);

                    return e->mapper.get_type_for(derived->canonical_name());
                },
                engine, n
            };
        return {};
    }

    any InferenceVisitor::visit(Cast* n)
    {
        n->left->visit(this);
        n->right->visit(this);

        engine->inferred_types[n] =
            InferredType {
                [](TypeEngine* e, ASTNode* node)
                {
                    auto derived = static_cast<Cast*>(node);

                    return e->mapper.get_type_for(derived->right->canonical_name());
                },
                engine, n
            };
        return {};
    }

    any InferenceVisitor::visit(TypeCheck* n)
    {
        n->left->visit(this);
        n->right->visit(this);

        engine->inferred_types[n] =
            InferredType {
                [](TypeEngine* e, ASTNode* node)
                {
                    return SimpleType(BasicType::Boolean);
                },
                engine, n
            };

        return {};
    }

    any InferenceVisitor::visit(Alias* n)
    {
        // Aliases only affect the symbol table, and they are not hoisted; an alias
        // only applies to code in the current scope (and its children) following
        // the point of declaration.
        module_scope->add_symbol(n->alias->canonical_name(), n);

        engine->mapper.add_type_definition(
            n->alias->canonical_name(),
            engine->mapper.get_type_for(n->original->canonical_name())
        );
        
        return {};
    }

    any InferenceVisitor::visit(Enum* n)
    {
        n->name->visit(this);
        n->values->visit(this);
        
        // TODO: Anything else? Add to mapper or something?

        return {};
    }

    any InferenceVisitor::visit(SymbolList* n)
    {
        for (auto&& ch : n->symbols)
        {
            ch->visit(this);
        }

        // TODO: Anything else?

        return {};
    }

    // Most statements don't have types themselves, but they'll still need to descend
    // into their child nodes for the expressions.
    any InferenceVisitor::visit(If* n)
    {
        n->condition->visit(this);
        n->then_case->visit(this);
        n->else_case->visit(this);

        return {};
    }

    any InferenceVisitor::visit(BareExpression* n)
    {
        n->expression->visit(this);

        return {};
    }

    any InferenceVisitor::visit(Block* n)
    {
        for (auto&& ch : n->children)
        {
            ch->visit(this);
        }

        return {};
    }

    any InferenceVisitor::visit(While* n)
    {
        n->condition->visit(this);
        n->body->visit(this);
        return {};
    }

    any InferenceVisitor::visit(For* n)
    {
        n->range->visit(this);
        n->body->visit(this);

        // For loops introduce a loop variable, so we have to account for that.
        // As it's stored as a string rather than a node pointer, we'll use this
        // node's pointer as the index into the map.
        engine->inferred_types[n] =
            InferredType {
                [](TypeEngine* e, ASTNode* node)
                {
                    auto derived = static_cast<For*>(node);

                    return e->inferred_types[derived->body.get()]();
                },
                engine, n
            };

        return {};
    }

    any InferenceVisitor::visit(With* n)
    {
        n->body->visit(this);

        for (auto&& ch : n->predicates)
        {
            ch->visit(this);
        }

        return {};
    }

    any InferenceVisitor::visit(Break* n)
    {
        // Breaks need no type inference
        return {};
    }

    any InferenceVisitor::visit(Continue* n)
    {
        // Continues need no type inference
        return {};
    }

    any InferenceVisitor::visit(Match* n)
    {
        n->expression->visit(this);

        for (auto&& ch : n->cases)
        {
            ch->visit(this);
        }

        return {};
    }

    any InferenceVisitor::visit(On* n)
    {
        n->case_expr->visit(this);
        n->body->visit(this);
        return {};
    }

    any InferenceVisitor::visit(When* n)
    {
        n->predicate->visit(this);
        n->body->visit(this);
        return {};
    }

    any InferenceVisitor::visit(TypeCase* n)
    {
        n->type_name->visit(this);
        n->body->visit(this);
        return {};
    }

    any InferenceVisitor::visit(Default* n)
    {
        // Defaults need no type inference
        return {};
    }

    any InferenceVisitor::visit(PredicateCall* n)
    {
        n->target->visit(this);

        for (auto&& ch : n->arguments)
        {
            ch->visit(this);
        }

        engine->inferred_types[n] =
            InferredType {
                [](TypeEngine* e, ASTNode* node)
                {
                    return SimpleType(BasicType::Boolean);
                },
                engine, n
            };

        return {};
    }

    any InferenceVisitor::visit(TypeDeclaration* n)
    {
        n->lhs->visit(this);
        n->rhs->visit(this);

        module_scope->add_symbol(n->lhs->canonical_name(), n);

        engine->inferred_types[n] =
            InferredType {
                [](TypeEngine* e, ASTNode* node)
                {
                    auto derived = static_cast<TypeDeclaration*>(node);

                    return e->mapper.get_type_for(derived->rhs->canonical_name());
                },
                engine, n
            };

        return {};
    }

    any InferenceVisitor::visit(Variable* n)
    {
        n->lhs->visit(this);
        n->rhs->visit(this);

        module_scope->add_symbol(n->lhs->canonical_name(), n);

        engine->inferred_types[n] =
            InferredType {
                [](TypeEngine* e, ASTNode* node)
                {
                    auto derived = static_cast<Variable*>(node);

                    if (e->inferred_types.count(derived->rhs.get()) != 0)
                    {
                        return e->inferred_types[derived->rhs.get()]();
                    }
                    else
                    {
                        return TypeInfo {UnknownType()};
                    }
                },
                engine, n
            };
        return {};
    }

    any InferenceVisitor::visit(Constant* n)
    {
        n->lhs->visit(this);
        n->rhs->visit(this);

        module_scope->add_symbol(n->lhs->canonical_name(), n);

        engine->inferred_types[n] =
            InferredType {
                [](TypeEngine* e, ASTNode* node)
                {
                    auto derived = static_cast<Constant*>(node);

                    if (e->inferred_types.count(derived->rhs.get()) != 0)
                    {
                        return e->inferred_types[derived->rhs.get()]();
                    }
                    else
                    {
                        return TypeInfo {UnknownType()};
                    }
                },
                engine, n
            };
            
        return {};
    }

}}
