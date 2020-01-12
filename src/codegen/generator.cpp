#include "codegen/generator.hpp"

namespace rhea { namespace codegen {
    CodeGenerator::CodeGenerator() : visitor(this), context(), builder(context), 
        module(std::make_unique<llvm::Module>("main", context)),
        FPM({}), FAM({})
    {
        initialize_passes();
        initialize_module();
    }

    CodeGenerator::CodeGenerator(std::string module) : visitor(this), context(), builder(context),
        module(std::make_unique<llvm::Module>(module, context)),
        FPM({}), FAM({})
    {
        initialize_passes();
        initialize_module();
    }

    void CodeGenerator::initialize_passes()
    {
        llvm::PassBuilder PB;
        PB.registerFunctionAnalyses(FAM);

        FPM = PB.buildFunctionSimplificationPipeline(
            llvm::PassBuilder::OptimizationLevel::O1,
            llvm::PassBuilder::ThinLTOPhase::None
        );
    }

    void CodeGenerator::initialize_module()
    {
        auto ft = llvm::FunctionType::get(
            llvm::Type::getVoidTy(context),
            false
        );

        auto fn = llvm::Function::Create(
            ft,
            llvm::Function::ExternalLinkage,
            module->getModuleIdentifier() + "_init",
            module.get()
        );

        auto block = llvm::BasicBlock::Create(context, "entry", fn);
        builder.SetInsertPoint(block);
    }

    llvm::Type* CodeGenerator::llvm_for_type(types::TypeInfo ti)
    {
        return util::visit(TypeBuilder{this}, ti);
    }

    // Specializations for the type converter
    template <>
    llvm::Type* TypeBuilder::operator()(types::SimpleType st)
    {
        using types::BasicType;
    
        switch (st.type)
        {
            // LLVM makes no distinction between signed/unsigned
            case BasicType::Integer:
            case BasicType::UnsignedInteger:
                return llvm::Type::getInt32Ty(generator->context);
            case BasicType::Byte:
            case BasicType::UnsignedByte:
                return llvm::Type::getInt8Ty(generator->context);
            case BasicType::Long:
            case BasicType::UnsignedLong:
                return llvm::Type::getInt64Ty(generator->context);
            case BasicType::Float:
                return llvm::Type::getFloatTy(generator->context);
            case BasicType::Double:
                return llvm::Type::getDoubleTy(generator->context);
            case BasicType::Boolean:
                return llvm::Type::getInt1Ty(generator->context);

            default:
                // TODO: Handle other types
                return nullptr;
        }
    }
}}