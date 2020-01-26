#include "codegen/generator.hpp"

namespace rhea { namespace codegen {
    CodeGenerator::CodeGenerator() : visitor(this), context(), builder(context), 
        module(std::make_unique<llvm::Module>("main", context)),
        FPM({}), FAM({})
    {
        initialize_passes();
    }

    CodeGenerator::CodeGenerator(std::string module) : visitor(this), context(), builder(context),
        module(std::make_unique<llvm::Module>(module, context)),
        FPM({}), FAM({})
    {
        initialize_passes();
    }

    util::any CodeGenerator::generate(ast::ASTNode* tree)
    {
        initialize_module();

        auto result = tree->visit(&visitor);

        finalize_module();

        return result;
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
        llvm::InitializeNativeTarget();
        // llvm::InitializeAllTargets();
        // llvm::InitializeAllTargetMCs();
        llvm::InitializeNativeTargetAsmParser();
        llvm::InitializeNativeTargetAsmPrinter();

        auto target_triple = llvm::sys::getDefaultTargetTriple();
        std::string error;
        auto target = llvm::TargetRegistry::lookupTarget(target_triple, error);
        if (!target)
        {
            llvm::errs() << error;
            throw std::invalid_argument(error);
        }

        auto cpu = "generic";
        auto features = "";
        llvm::TargetOptions opts;
        auto reloc_model = llvm::Optional<llvm::Reloc::Model>();
        target_machine = target->createTargetMachine(target_triple, cpu, features, opts, reloc_model);

        module->setDataLayout(target_machine->createDataLayout());
        module->setTargetTriple(target_triple);

        // Set up a top-level function
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

    void CodeGenerator::finalize_module()
    {
        auto fname = module->getModuleIdentifier() + "_init";

        auto fn = module->getFunction(fname);
        if (fn != nullptr)
        {
            builder.CreateRetVoid();
            llvm::verifyFunction(*fn);
        }
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