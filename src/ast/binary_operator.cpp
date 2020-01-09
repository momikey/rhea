#include "ast.hpp"
#include "types/types.hpp"

namespace rhea { namespace ast {
    types::TypeInfo BinaryOp::expression_type()
    {
        auto lt = left->expression_type();
        auto rt = right->expression_type();
        if (types::compatible(lt, rt))
        {
            return lt;
        }
        
        return types::UnknownType();
    }
}}