#include "ast/nodes/function.hpp"

#include <algorithm>
#include <iterator>

namespace rhea { namespace ast {
    // Function call nodes can take two different types of argument list.
    // The constructor logic is the same for both, as Boost and standard
    // variants have implicit constructors.

    Call::Call(expression_ptr t, child_vector<Expression>& args)
        : target(std::move(t))
    {
        std::move(args.begin(), args.end(), std::back_inserter(arguments));
    }

    Call::Call(expression_ptr t, child_vector<NamedArgument>& args)
        : target(std::move(t))
    {
        std::move(args.begin(), args.end(), std::back_inserter(arguments));
    }

    std::string Call::to_string()
    {
        return fmt::format("(Call,{0}{1})",
            target->to_string(),
            util::serialize_array(arguments));
    }

    Arguments::Arguments(child_vector<TypePair>& args)
    {
        std::move(args.begin(), args.end(), std::back_inserter(arguments));
    }

    std::string Arguments::to_string()
    {
        return fmt::format("(Arguments{0})", util::serialize_array(arguments));  
    }

    Def::Def(FunctionType t, std::string n, std::unique_ptr<Typename> rt,
        std::unique_ptr<Arguments> al, child_vector<Condition>& cs, statement_ptr b)
        : type(t), name(n), return_type(std::move(rt)),
          arguments_list(std::move(al)), body(std::move(b))
    {
        std::move(cs.begin(), cs.end(), std::back_inserter(conditions));
    }

    std::string Def::to_string()
    {
        auto cstr = fmt::format("(Conditions{0})", util::serialize_array(conditions));

        return fmt::format("(Def,{0},{1},{2},{3},{4},{5})",
            static_cast<int>(type),
            name,
            return_type != nullptr ? return_type->to_string() : "null",
            arguments_list != nullptr ? arguments_list->to_string() : "null",
            cstr,
            body->to_string()
        );
    }

    GenericDef::GenericDef(
        FunctionType t,
        std::string n,
        std::vector<GenericMatch>& gt,
        std::unique_ptr<Typename> rt,
        std::unique_ptr<Arguments> al,
        child_vector<Condition>& cs,
        statement_ptr b
    ) : Def(t, n, std::move(rt), std::move(al), cs, std::move(b))
    {
        // The only thing a GenericDef has to do differently is account for
        // the generic typenames. These are variants, so...fun.
        std::move(gt.begin(), gt.end(), std::back_inserter(generic_types));
    }

    std::string GenericDef::to_string()
    {
        return fmt::format("(Def,{0},{1},(GenericTypes{2}),{3},{4},(Conditions{5}),{6})",
            static_cast<int>(type),
            name,
            util::serialize_array(generic_types),
            return_type != nullptr ? return_type->to_string() : "null",
            arguments_list != nullptr ? arguments_list->to_string() : "null",
            util::serialize_array(conditions),
            body->to_string()
        );
    }

    std::string Def::function_type_string()
    {
        std::string type_suffix;
        switch (type)
        {
            case FunctionType::Predicate:
                type_suffix = "?";
                break;
            case FunctionType::Operator:
                type_suffix = "$";
                break;
            case FunctionType::Unchecked:
                type_suffix = "!";
                break;
            default:
                break;
        }

        std::string args;
        if (arguments_list != nullptr)
        {
            for (auto&& a : arguments_list->arguments)
            {
                args += a->value->canonical_name();
                args += ",";
            }
            args.pop_back();            
        }

        return fmt::format("{0}{1} <{2}> -> {3}",
            name,
            type_suffix,
            args,
            return_type != nullptr ? return_type->canonical_name() : "nothing"
        );
    }
}}