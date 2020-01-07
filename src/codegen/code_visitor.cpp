#include "codegen/code_visitor.hpp"
#include "codegen/generator.hpp"

namespace rhea { namespace codegen {
    using namespace rhea::ast;
    using llvm::Value;

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
}}