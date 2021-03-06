#include "codegen/code_visitor.hpp"
#include "codegen/generator.hpp"

namespace rhea { namespace codegen {
    using namespace rhea::ast;
    using llvm::Value;

    // Helper functions to reduce repetition. If we break the methods out into
    // their own files in the future (which may be necessary), we'll need to
    // move these into their own source/header pair.
    namespace internal {
        bool is_unsigned_type(types::BasicType t)
        {
            return
                t == BasicType::UnsignedByte ||
                t == BasicType::UnsignedInteger ||
                t == BasicType::UnsignedLong
            ;
        }

        bool is_floating_point_type(types::BasicType t)
        {
            return t == BasicType::Float || t == BasicType::Double;
        }

        template <typename Int, int Width, bool Signed>
        Value* integral_value(Int val, CodeGenerator* gen)
        {
            return llvm::ConstantInt::get(
                gen->context,
                llvm::APInt(Width, val, Signed)
            );
        }

        template <typename Float>
        Value* fp_value(Float val, CodeGenerator* gen)
        {
            return llvm::ConstantFP::get(
                gen->context,
                llvm::APFloat(val)
            );
        }

        llvm::AllocaInst* create_allocation(
            llvm::Function* fn, std::string name, types::TypeInfo type, CodeGenerator* gen)
        {
            if (gen->scope_manager.is_local(name))
            {
                // The variable does exist in the local scope, so we can allocate
                // stack space for it.

                // Create a temporary IR builder at the beginning of the scope block
                llvm::IRBuilder<> temp_builder(
                    &fn->getEntryBlock(),
                    fn->getEntryBlock().begin()
                );

                auto ai = temp_builder.CreateAlloca(gen->llvm_for_type(type), 0, name);

                return ai;
            }
            else
            {
                // Throw an error or something here
                return nullptr;
            }
        }
    }

    any CodeVisitor::visit(Boolean* n)
    {
        Value* ret;
        if (n->value)
        {
            ret = llvm::ConstantInt::getTrue(generator->context);
        }
        else
        {
            ret = llvm::ConstantInt::getFalse(generator->context);
        }

        return ret;
    }

    any CodeVisitor::visit(Integer* n)
    {
        auto ret = internal::integral_value<
            Integer::template_type,
            32,     // TODO: Magic number
            true
        >(n->value, generator);

        return ret;
    }

    any CodeVisitor::visit(Byte* n)
    {
        auto ret = internal::integral_value<
            Byte::template_type,
            8,      // TODO: Magic number
            true
        >(n->value, generator);

        return ret;
    }

    any CodeVisitor::visit(Long* n)
    {
        auto ret = internal::integral_value<
            Long::template_type,
            64,     // TODO: Magic number
            true
        >(n->value, generator);

        return ret;
    }

    any CodeVisitor::visit(UnsignedInteger* n)
    {
        auto ret = internal::integral_value<
            UnsignedInteger::template_type,
            32,     // TODO: Magic number
            false
        >(n->value, generator);

        return ret;
    }

    any CodeVisitor::visit(UnsignedByte* n)
    {
        auto ret = internal::integral_value<
            UnsignedByte::template_type,
            8,      // TODO: Magic number
            false
        >(n->value, generator);

        return ret;
    }

    any CodeVisitor::visit(UnsignedLong* n)
    {
        auto ret = internal::integral_value<
            UnsignedLong::template_type,
            64,     // TODO: Magic number
            false
        >(n->value, generator);

        return ret;
    }

    any CodeVisitor::visit(Float* n)
    {
        auto ret = internal::fp_value<
            Float::template_type
        >(n->value, generator);

        return ret;
    }

    any CodeVisitor::visit(Double* n)
    {
        auto ret = internal::fp_value<
            Double::template_type
        >(n->value, generator);

        return ret;
    }

    any CodeVisitor::visit(Symbol* n)
    {
        auto ret = internal::integral_value<
            uint64_t,
            64,
            false
        >(util::symbol_hash(n->value), generator);

        return ret;
    }

    any CodeVisitor::visit(Nothing* n)
    {
        // Maybe change this, since some functions can return an actual nothing
        // instead of just being void.
        Value* ret = llvm::ConstantTokenNone::get(generator->context);

        return ret;
    }

    any CodeVisitor::visit(Identifier* n)
    {
        // This visitor implements read access for identifiers. Assignment and
        // variable definition handle write access in their own way.

        Value* ret = nullptr;

        // Note: this is an optional possibly holding a reference_wrapper.
        auto varopt = generator->scope_manager.find(n->name);

        if (varopt)
        {
            // Now we can actually unpack the search result.
            state::SymbolEntry var;
            std::string scope;
            std::tie(var, scope) = varopt.value();

            switch (var.declaration)
            {
                case types::DeclarationType::Variable:
                case types::DeclarationType::Constant:
                {
                    if (scope == "$global")
                    {
                        // Global variables are accessed differently.
                        auto gvar = generator->module->getGlobalVariable(var.name, true);
                        ret = generator->builder.CreateLoad(gvar, var.name);
                    }
                    else
                    {
                        // Local variables need a load instruction with the proper address.
                        auto lvar = generator->allocation_manager.find(var.name);
                        if (lvar)
                        {
                            ret = generator->builder.CreateLoad(*lvar, var.name);
                        }
                        else
                        {
                            throw std::invalid_argument("Variable " + var.name + " not in allocation table");
                        }
                    }

                    n->set_expression_type(var.type_data);

                    break;
                }
                default:
                    throw unimplemented_type("Variable declaration " + std::to_string(static_cast<int>(var.declaration)));
            }
        }
        else
        {
            // Variable is not defined
            throw syntax_error("Identifier " + n->name + " is not defined");
        }

        return ret;
    }

    any CodeVisitor::visit(BinaryOp* n)
    {
        using ast::BinaryOperators;

        Value* lhs = util::any_cast<Value*>(n->left->visit(this));
        Value* rhs = util::any_cast<Value*>(n->right->visit(this));

        // The type of the whole expression
        auto et = n->expression_type().type();
        auto as_simple = util::get_if<types::SimpleType>(&et);

        // The types of the operands
        auto lt = n->left->expression_type().type();
        auto lt_simple = util::get_if<types::SimpleType>(&lt);

        auto rt = n->right->expression_type().type();
        auto rt_simple = util::get_if<types::SimpleType>(&rt);

        // Not allowed to use the coercion operator on the LHS
        if (lt_simple != nullptr && lt_simple->type == BasicType::Promoted)
        {
            throw syntax_error("Can't use the coercion operator on the left-hand side of an expression");
        }

        // Convert the type of the RHS, if necessary

        // TODO: Handle casting operations
        auto coerce = (rt_simple != nullptr && rt_simple->type == BasicType::Promoted);
        if (!coerce)
        {
            rhs = convert_type(generator, rhs, rt, lt, false);
        }
        else
        {
            // If this is an explicit conversion from the coercion operator
            auto ty = (dynamic_cast<ast::UnaryOp*>(n->right.get()))->operand->expression_type();
            rhs = convert_type(generator, rhs, ty, lt, true);
        }

        Value* ret = nullptr;

        if (as_simple != nullptr)
        {
            // Operations on simple types
            if (as_simple->is_integral ||
                (as_simple->type == BasicType::Boolean && lt_simple->is_integral)
            )
            {
                // Operations on integer types
                switch (n->op)
                {
                    case BinaryOperators::Add:
                        ret = generator->builder.CreateAdd(lhs, rhs, "addtmp");
                        break;
                    case BinaryOperators::Subtract:
                        ret = generator->builder.CreateSub(lhs, rhs, "subtmp");
                        break;
                    case BinaryOperators::Multiply:
                        ret = generator->builder.CreateMul(lhs, rhs, "multmp");
                        break;
                    case BinaryOperators::Divide:
                        if (internal::is_unsigned_type(as_simple->type))
                        {
                            ret = generator->builder.CreateUDiv(lhs, rhs, "divtmp");
                        }
                        else
                        {
                            ret = generator->builder.CreateSDiv(lhs, rhs, "divtmp");
                        }
                        break;
                    case BinaryOperators::Modulus:
                        if (internal::is_unsigned_type(as_simple->type))
                        {
                            ret = generator->builder.CreateURem(lhs, rhs, "modtmp");
                        }
                        else
                        {
                            ret = generator->builder.CreateSRem(lhs, rhs, "modtmp");
                        }
                        break;
                    case BinaryOperators::Equals:
                        ret = generator->builder.CreateICmpEQ(lhs, rhs, "cmptmp");
                        break;
                    case BinaryOperators::NotEqual:
                        ret = generator->builder.CreateICmpNE(lhs, rhs, "cmptmp");
                        break;
                    case BinaryOperators::LessThan:
                        if (lt_simple != nullptr && internal::is_unsigned_type(lt_simple->type))
                        {
                            ret = generator->builder.CreateICmpULT(lhs, rhs, "cmptmp");
                        }
                        else
                        {
                            ret = generator->builder.CreateICmpSLT(lhs, rhs, "cmptmp");
                        }
                        break;
                    case BinaryOperators::LessThanOrEqual:
                        if (lt_simple != nullptr && internal::is_unsigned_type(lt_simple->type))
                        {
                            ret = generator->builder.CreateICmpULE(lhs, rhs, "cmptmp");
                        }
                        else
                        {
                            ret = generator->builder.CreateICmpSLE(lhs, rhs, "cmptmp");
                        }
                        break;
                    case BinaryOperators::GreaterThan:
                        if (lt_simple != nullptr && internal::is_unsigned_type(lt_simple->type))
                        {
                            ret = generator->builder.CreateICmpUGT(lhs, rhs, "cmptmp");
                        }
                        else
                        {
                            ret = generator->builder.CreateICmpSGT(lhs, rhs, "cmptmp");
                        }
                        break;
                    case BinaryOperators::GreaterThanOrEqual:
                        if (lt_simple != nullptr && internal::is_unsigned_type(lt_simple->type))
                        {
                            ret = generator->builder.CreateICmpUGE(lhs, rhs, "cmptmp");
                        }
                        else
                        {
                            ret = generator->builder.CreateICmpSGE(lhs, rhs, "cmptmp");
                        }
                        break;

                    // TODO: Others
                    default:
                        break;
                }
            }
            else if (internal::is_floating_point_type(as_simple->type) ||
                (as_simple->type == BasicType::Boolean && internal::is_floating_point_type(lt_simple->type))
            )
            {
                // Operations on floating-point types
                switch (n->op)
                {
                    case BinaryOperators::Add:
                        ret = generator->builder.CreateFAdd(lhs, rhs, "addtmp");
                        break;
                    case BinaryOperators::Subtract:
                        ret = generator->builder.CreateFSub(lhs, rhs, "subtmp");
                        break;
                    case BinaryOperators::Multiply:
                        ret = generator->builder.CreateFMul(lhs, rhs, "multmp");
                        break;
                    case BinaryOperators::Divide:
                        ret = generator->builder.CreateFDiv(lhs, rhs, "divtmp");
                        break;
                    case BinaryOperators::Modulus:
                        ret = generator->builder.CreateFRem(lhs, rhs, "modtmp");
                        break;
                    case BinaryOperators::Equals:
                        ret = generator->builder.CreateFCmpOEQ(lhs, rhs, "cmptmp");
                        break;
                    case BinaryOperators::NotEqual:
                        ret = generator->builder.CreateFCmpONE(lhs, rhs, "cmptmp");
                        break;
                    case BinaryOperators::LessThan:
                        ret = generator->builder.CreateFCmpOLT(lhs, rhs, "cmptmp");
                        break;
                    case BinaryOperators::LessThanOrEqual:
                        ret = generator->builder.CreateFCmpOLE(lhs, rhs, "cmptmp");
                        break;
                    case BinaryOperators::GreaterThan:
                        ret = generator->builder.CreateFCmpOGT(lhs, rhs, "cmptmp");
                        break;
                    case BinaryOperators::GreaterThanOrEqual:
                        ret = generator->builder.CreateFCmpOGE(lhs, rhs, "cmptmp");
                        break;
                    // TODO: Others
                    default:
                        break;
                }
            }
            else if (as_simple->type == BasicType::Boolean && lt_simple->type == BasicType::Boolean)
            {
                // Boolean operations
                switch (n->op)
                {
                    case BinaryOperators::BooleanAnd:
                        ret = generator->builder.CreateAnd(lhs, rhs, "andtmp");
                        break;
                    case BinaryOperators::BooleanOr:
                        ret = generator->builder.CreateOr(lhs, rhs, "ortmp");
                        break;
                    default:
                        // Other operations are not defined on boolean values,
                        // so should we throw an error here?
                        // throw syntax_error("Invalid type for operation");
                        break;
                }
            }
            else
            {
                // Operations on other simple types
            }
        }
        // TODO: Handle other types: structures, refs, pointers, etc.

        return ret;
    }

    any CodeVisitor::visit(UnaryOp* n)
    {
        using ast::UnaryOperators;

        Value* operand = util::any_cast<Value*>(n->operand->visit(this));

        // The type of the whole expression
        auto et = n->expression_type().type();
        auto as_simple = util::get_if<types::SimpleType>(&et);

        // The type of the operand, which we will need for, e.g., promotion
        auto ot = n->operand->expression_type().type();
        auto ot_simple = util::get_if<types::SimpleType>(&ot);

        Value* ret = nullptr;

        if (as_simple != nullptr)
        {
            switch (n->op)
            {
                case UnaryOperators::Plus:
                    if (as_simple->is_numeric)
                    {
                        // No need for code output, but we have to provide *something*
                        // for basic block construction.
                        ret = generator->builder.CreateSelect(
                            llvm::ConstantInt::getTrue(generator->context),
                            operand,
                            llvm::UndefValue::get(operand->getType())
                        );
                    }
                    else
                    {
                        // Can't use unary plus on non-numeric builtin types
                        throw syntax_error("Invalid type for unary plus operator");
                    }
                case UnaryOperators::Minus:
                    if (as_simple->is_numeric)
                    {
                        // LLVM IR doesn't have a basic negation instruction, AFAIK
                        if (as_simple->is_integral)
                        {
                            ret = generator->builder.CreateSub(
                                llvm::ConstantInt::get(operand->getType(), 0),
                                operand
                            );
                        }
                        else
                        {
                            ret = generator->builder.CreateFSub(
                                llvm::ConstantFP::get(operand->getType(), 0),
                                operand
                            );
                        }
                    }
                    else
                    {
                        // Can't use unary minus on non-numeric builtin types
                        throw syntax_error("Invalid type for unary minus operator");
                    }
                case UnaryOperators::BooleanNot:
                    if (as_simple->type == BasicType::Boolean)
                    {
                        // Again, no builtin "not" instruction for LLVM IR
                        ret = generator->builder.CreateSub(
                            llvm::ConstantInt::getTrue(generator->context),
                            operand,
                            "nottmp"
                        );
                    }
                    else
                    {
                        throw syntax_error("Invalid type for not operator");
                    }
                case UnaryOperators::Coerce:
                    // Coercion operator doesn't actually generate code. All it does
                    // is ensure type safety.
                    ret = operand;
                default:
                    break;
            }
        }
        else
        {
            // TODO: Operations on other types
        }

        return ret;
    }

    any CodeVisitor::visit(TernaryOp* n)
    {
        // The ternary operator in Rhea is an expression. Luckily for us,
        // LLVM provides a `select` instruction that does pretty much exactly
        // what we want.

        // First get the condition expression as a boolean
        Value* cond = util::any_cast<Value*>(n->condition->visit(this));
        cond = convert_type(generator, cond, n->condition->expression_type(), BasicType::Boolean, false);

        Value* t_branch = util::any_cast<Value*>(n->true_branch->visit(this));
        Value* f_branch = util::any_cast<Value*>(n->false_branch->visit(this));

        auto tbt = n->true_branch->expression_type();
        auto fbt = n->false_branch->expression_type();
        auto fbt_simple = util::get_if<types::SimpleType>(&(fbt.type()));

        // Implicit conversion of the false branch, because the true branch
        // controls the result type.

        auto coerce = (fbt_simple != nullptr && fbt_simple->type == BasicType::Promoted);
        if (!coerce)
        {
            f_branch = convert_type(generator, f_branch, fbt, tbt, false);
        }
        else
        {
            // Explicit conversion from the coercion operator
            auto ty = (dynamic_cast<ast::UnaryOp*>(n->false_branch.get()))->operand->expression_type();
            f_branch = convert_type(generator, f_branch, ty, tbt, true);
        }

        Value* ret = generator->builder.CreateSelect(
            cond, t_branch, f_branch, "ternaryop"
        );

        // TODO Anything else?
        return ret;
    }

    any CodeVisitor::visit(BareExpression* n)
    {
        // Bare expressions aren't really statements, but expressions evaluated
        // in a statement context. They're most useful for function calls, which
        // may have side effects. In codegen, we don't need to do much with the
        // statement itself, so we wrap it in what's basically an IIFE.

        // static unsigned int count = 0u;

        auto value = util::any_cast<Value*>(n->expression->visit(this));

        auto result = generator->builder.CreateSelect(
            llvm::ConstantInt::getTrue(generator->context),
            value,
            llvm::UndefValue::get(value->getType())
        );

        return result;
        // If we're at the top level of a module/program, we can't directly
        // insert the expression, so we wrap it in an anonymous function.
        // llvm::Function* ret = nullptr;

        // auto fntype = llvm::FunctionType::get(
        //     value->getType(),
        //     false
        // );

        // ret = llvm::Function::Create(
        //     fntype,
        //     llvm::Function::ExternalLinkage,
        //     std::string("__anon_expr$") + std::to_string(count++),
        //     generator->module.get()
        // );

        // auto block = llvm::BasicBlock::Create(generator->context, "entry", ret);

        // // Save position for later
        // auto old = generator->builder.GetInsertBlock();

        // // Move to new function
        // generator->builder.SetInsertPoint(block);

        // if (value != nullptr)
        // {
        //     generator->builder.CreateRet(value);
        //     llvm::verifyFunction(*ret);

        //     generator->builder.SetInsertPoint(old);
        //     generator->builder.CreateCall(ret, llvm::None, "callanon");
        //     return ret;
        // }
        // else
        // {
        //     ret->eraseFromParent();
        //     return nullptr;
        // }
    }

    any CodeVisitor::visit(Block* n)
    {
        // Blocks do not produce IR basic blocks, but they do introduce a new declaration scope.
        
        // First get a name for the scope. We create it from the name of the parent IR block.
        llvm::Function* parent_fn = generator->builder.GetInsertBlock()->getParent();
        std::string scope_name = parent_fn->getName().str() + "_block";
        generator->create_scope(scope_name);

        // Blocks have no real return values, but we'll do like a lot of other languages
        // and keep the value of the last statement in the block.
        util::any ret;

        // Codegen for the block is easy: loop through all the statements.
        for (auto& statement : n->children)
        {
            ret = statement->visit(this);
        }

        // Destroy the scope at the end.
        generator->destroy_scope();

        return ret;
    }

    any CodeVisitor::visit(If* n)
    {
        util::any ret;

        // Get the condition expression as a boolean (no implicit conversion in Rhea)
        auto cond = util::any_cast<Value*>(n->condition->visit(this));
        cond = convert_type(generator, cond, n->condition->expression_type(), BasicType::Boolean, false);

        // Create blocks for the cases, then the "merge" block at the end
        llvm::Function* parent_fn = generator->builder.GetInsertBlock()->getParent();

        llvm::BasicBlock* then_block = llvm::BasicBlock::Create(generator->context, "then", parent_fn);
        llvm::BasicBlock* else_block = llvm::BasicBlock::Create(generator->context, "else");
        llvm::BasicBlock* merge_block = llvm::BasicBlock::Create(generator->context, "ifmerge");

        generator->builder.CreateCondBr(cond, then_block, else_block);

        // Codegen for the "then" case
        generator->builder.SetInsertPoint(then_block);

        // "Unless" statements won't have a then case, and it's possible that
        // normal ifs won't, either.
        if (n->then_case != nullptr)
        {
            util::any then_result = n->then_case->visit(this);
            if (then_result.empty())    // Change to !has_value() for C++17 any
            {
                // Something went wrong with codegen, but not enough to throw an error
                return nullptr;
            }

            ret = then_result;
        }

        generator->builder.CreateBr(merge_block);
        // Restore the insert block if the then condition changed it
        then_block = generator->builder.GetInsertBlock();

        // Now set up for the "else" case
        parent_fn->getBasicBlockList().push_back(else_block);
        generator->builder.SetInsertPoint(else_block);

        // Not all ifs will have elses.
        if (n->else_case != nullptr)
        {
            util::any else_result = n->else_case->visit(this);
            if (else_result.empty())    // Change to !has_value() for C++17 any
            {
                // Something went wrong with codegen, but not enough to throw an error
                return nullptr;
            }

            ret = else_result;
        }

        // Same as above...
        generator->builder.CreateBr(merge_block);
        // Restore the insert block if the else condition changed it
        else_block = generator->builder.GetInsertBlock();

        // Now merge the two code paths
        parent_fn->getBasicBlockList().push_back(merge_block);
        generator->builder.SetInsertPoint(merge_block);

        // Note that we don't need an explicit Phi node, because Rhea if/else
        // doesn't return a value.

        return ret;
    }

    any CodeVisitor::visit(TypeDeclaration* n)
    {
        // For a variable declaration (with no initialization), we only have to
        // place an entry in the current scope's symbol table and allocate stack
        // memory for the appropriate variable type.

        std::string var_name = n->lhs->name;
        std::string type_name = n->rhs->canonical_name();

        if (!generator->type_mapper.is_type_defined(type_name))
        {
            throw syntax_error(fmt::format("Undefined type '{0}' in declaration", type_name));
        }

        auto vtype = generator->type_mapper.get_type_for(type_name);
        auto ltype = generator->llvm_for_type(vtype);

        Value* result = nullptr;

        // Only declare the new variable if one doesn't exist in the current scope.
        if (!generator->scope_manager.is_local(var_name))
        {
            generator->scope_manager.add_symbol({
                var_name,
                types::DeclarationType::Variable,
                vtype
            });

            if (generator->scope_manager.current().name != "$global")
            {
                // We're in some kind of a block, such as a function definition.
                llvm::Function* entry = generator->builder.GetInsertBlock()->getParent();

                auto ai = internal::create_allocation(entry, var_name, vtype, generator);
                generator->allocation_manager.add(var_name, ai);

                result = generator->builder.GetInsertBlock()->getParent();
            }
            else
            {
                // This is a top-level variable definition, so treat it as a global.

                // LLVM likes to take ownership of...well, everything. So we can't
                // use a unique_ptr here, apparently.
                auto gvar = new llvm::GlobalVariable(
                    *(generator->module),
                    ltype,
                    false,
                    llvm::GlobalVariable::LinkageTypes::InternalLinkage,
                    llvm::Constant::getNullValue(ltype),
                    var_name                    
                );

                result = gvar;
            }
        }
        else
        {
            throw syntax_error(fmt::format("Redeclaration of variable {0} with type '{1}'", var_name, type_name));
        }

        return result;
    }

    any CodeVisitor::visit(Variable* n)
    {
        // For a variable definition (with initialization), we also have to store
        // the RHS expression's value into the appropriate memory.

        std::string vname = n->lhs->name;

        Value* rhs = util::any_cast<Value*>(n->rhs->visit(this));
        auto vtype = n->rhs->expression_type();
        auto ltype = generator->llvm_for_type(vtype);

        if (!generator->scope_manager.is_local(vname))
        {
            generator->scope_manager.add_symbol({
                vname,
                types::DeclarationType::Variable,
                vtype
            });

            if (generator->scope_manager.current().name != "$global")
            {
                // We're in some kind of a block, such as a function definition.
                llvm::Function* entry = generator->builder.GetInsertBlock()->getParent();

                // if (entry == nullptr)
                // {
                //     // This is possibly some kind of top-level block.
                // }

                auto ai = internal::create_allocation(entry, vname, vtype, generator);

                generator->builder.CreateStore(rhs, ai);
                generator->allocation_manager.add(vname, ai);

                return generator->builder.GetInsertBlock()->getParent();
            }
            else
            {
                // This is a top-level variable definition, so treat it as a global.

                // LLVM likes to take ownership of...well, everything. So we can't
                // use a unique_ptr here, apparently.
                auto gvar = new llvm::GlobalVariable(
                    *(generator->module),
                    ltype,
                    false,
                    llvm::GlobalVariable::LinkageTypes::InternalLinkage,
                    llvm::Constant::getNullValue(ltype),
                    vname                    
                );

                generator->builder.CreateStore(rhs, gvar);

                // Don't really know what to return here, so hand off to the outer block.
            }
        }
        else
        {
            // Redefinition
            throw syntax_error(fmt::format("Redefinition of variable {0}", vname));
        }

        return {};
    }

    any CodeVisitor::visit(Constant* n)
    {
        // For now, we just use the same code for constants. Later, we might be able
        // to optimize this. Remember that Rhea's var/const distinction is more like
        // that of JavaScript. A constant doesn't have to be known at compile time.
        std::string vname = n->lhs->name;
        auto vtype = n->rhs->expression_type();
        auto ltype = generator->llvm_for_type(vtype);
        Value* rhs = util::any_cast<Value*>(n->rhs->visit(this));

        if (!generator->scope_manager.is_local(vname))
        {
            generator->scope_manager.add_symbol({
                vname,
                types::DeclarationType::Constant,
                vtype
            });

            if (generator->scope_manager.current().name != "$global")
            {
                // We're in some kind of a block, such as a function definition.
                llvm::Function* entry = generator->builder.GetInsertBlock()->getParent();
                auto ai = internal::create_allocation(entry, vname, vtype, generator);

                generator->builder.CreateStore(rhs, ai);
                generator->allocation_manager.add(vname, ai);

                return generator->builder.GetInsertBlock()->getParent();
            }
            else
            {
                // This is a top-level variable definition, so treat it as a global.

                // LLVM likes to take ownership of...well, everything. So we can't
                // use a unique_ptr here, apparently.
                auto gvar = new llvm::GlobalVariable(
                    *(generator->module),
                    ltype,
                    false,
                    llvm::GlobalVariable::LinkageTypes::InternalLinkage,
                    llvm::Constant::getNullValue(ltype),
                    vname                    
                );

                generator->builder.CreateStore(rhs, gvar);

                // Don't really know what to return here, so hand off to the outer block.
            }
        }
        else
        {
            // Redefinition
            throw syntax_error(fmt::format("Redefinition of variable {0}", vname));
        }

        return {};
    }
}}