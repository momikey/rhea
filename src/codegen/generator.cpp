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

    void CodeGenerator::initialize_passes()
    {
        llvm::PassBuilder PB;
        PB.registerFunctionAnalyses(FAM);

        FPM = PB.buildFunctionSimplificationPipeline(
            llvm::PassBuilder::OptimizationLevel::O1,
            llvm::PassBuilder::ThinLTOPhase::None
        );
    }
}}