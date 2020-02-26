#include "types/name_mangle.hpp"

namespace rhea { namespace types {
    using rhea::ast::unimplemented_type;

    std::string mangle_function_name(std::string name, FunctionType function_type,
     FunctionClass function_class)
    {
        // Unchecked functions are unmangled functions
        if (function_class == FunctionClass::Unchecked)
        {
            return name;
        }

        std::string mangled = "_R";

        switch (function_class)
        {
            case FunctionClass::Function:
                mangled += "f";
                break;
            case FunctionClass::Predicate:
                mangled += "p";
                break;
            case FunctionClass::Operator:
                mangled += "o";
                break;
            default:
                throw unimplemented_type(name);
        }

        if (function_class == FunctionClass::Operator)
        {
            mangled += name;
        }
        else
        {
            mangled += fmt::format("{0}{1}", name.length(), name);
        }

        if (function_type.return_type != nullptr)
        {
            mangled += internal::mangle_argument_name(function_type.return_type);
        }
        else
        {
            // A function without a return type implictly returns nothing.
            mangled += "v";
        }

        if (function_type.argument_types.empty())
        {
            mangled += "0";
        }
        else
        {
            for (auto&& t : function_type.argument_types)
            {
                auto fn = [&] (auto const& e) { return internal::mangle_argument_name(e); };
                auto result = util::visit(fn, t.second->type());

                mangled += result;
            }
        }

        return mangled;
    }
}}