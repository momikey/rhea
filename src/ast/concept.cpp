#include "ast.hpp"

namespace rhea { namespace ast {
    FunctionCheck::FunctionCheck(std::string t, std::unique_ptr<AnyIdentifier> fn,
        FunctionType ft, child_vector<Typename>& fas, std::unique_ptr<Typename> rtn)
        : type_name(t), function_name(std::move(fn)), function_type(ft),
            return_type_name(std::move(rtn))
    {
        std::move(fas.begin(), fas.end(), std::back_inserter(function_arguments));
    }

    std::string FunctionCheck::to_string()
    {
        return fmt::format("(FunctionCheck,{0},{1},{2},{3}{4})",
            type_name,
            function_name->to_string(),
            function_type,
            return_type_name->to_string(),
            util::serialize_array(function_arguments)
        );
    }

    Concept::Concept(std::string n, std::string t, std::vector<ConceptCheck>& b)
        : name(n), type(t)
    {
        std::move(b.begin(), b.end(), std::back_inserter(body));
    }

    std::string Concept::to_string()
    {
        return fmt::format("(Concept,{0},{1}{2})",
            name,
            type,
            util::serialize_array(body)
        );
    }
}}