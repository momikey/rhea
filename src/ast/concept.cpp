#include "ast.hpp"

namespace rhea { namespace ast {
    FunctionCheck::FunctionCheck(std::string t, std::unique_ptr<AnyIdentifier> fn,
        child_vector<Typename>& fas, std::unique_ptr<Typename> rtn)
        : type_name(t), function_name(std::move(fn)), return_type_name(std::move(rtn))
    {
        std::move(fas.begin(), fas.end(), std::back_inserter(function_arguments));
    }

    std::string FunctionCheck::to_string()
    {
        return fmt::format("(FunctionCheck,{0},{1},{2}{3})",
            type_name,
            function_name->to_string(),
            return_type_name->to_string(),
            util::serialize_array(function_arguments)
        );
    }
}}