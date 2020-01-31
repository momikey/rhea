#include "ast.hpp"
#include "types/types.hpp"

namespace rhea { namespace ast {
    bool is_boolean_op(BinaryOperators op)
    {
        return
            op == BinaryOperators::Equals ||
            op == BinaryOperators::NotEqual ||
            op == BinaryOperators::LessThan ||
            op == BinaryOperators::GreaterThan ||
            op == BinaryOperators::LessThanOrEqual ||
            op == BinaryOperators::GreaterThanOrEqual ||
            op == BinaryOperators::BooleanAnd ||
            op == BinaryOperators::BooleanOr
        ;
    }
    
    types::TypeInfo BinaryOp::expression_type()
    {
        auto lt = left->expression_type();
        // auto rt = right->expression_type();
        // if (types::compatible(lt, rt))
        // {
            return (is_boolean_op(op)) ? types::SimpleType(types::BasicType::Boolean, false) : lt;
        // }
        
        // return types::UnknownType();
    }
}}