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
        // This is mostly the same logic, except that we have to worry
        // about visiting the variant.

        std::string s;
        s.reserve(arguments.size()*16);    // profile this to find a nice default

        for (auto&& id : arguments)
        {
            s += ',';
            s += visit([](auto const& v) {
                 return v->to_string(); }, id);
        }

        return fmt::format("(Call,{0}{1})", target->to_string(), s);
    }

    Arguments::Arguments(child_vector<TypePair>& args)
    {
        std::move(args.begin(), args.end(), std::back_inserter(arguments));
    }

    std::string Arguments::to_string()
    {
        // TODO: We'll need to add the visitor logic when we switch to variants.
        // See above for how to do that.
        std::string s;
        s.reserve(arguments.size()*16);    // profile this to find a nice default

        for (auto&& id : arguments)
        {
            s += ',';
            s += id->to_string();
        }

        return fmt::format("(Arguments{0})", s);  
    }

    Def::Def(FunctionType t, std::string n, std::unique_ptr<TypePair> gt,
        std::unique_ptr<Typename> rt, std::unique_ptr<Arguments> al, child_vector<Condition> cs,
        statement_ptr b)
        : type(t), name(n), generic_type(std::move(gt)), return_type(std::move(rt)),
          arguments_list(std::move(al)), body(std::move(b))
    {
        std::move(cs.begin(), cs.end(), std::back_inserter(conditions));
    }

    std::string Def::to_string()
    {
        // Only the conditions really need this part. Everything else is
        // handled by child nodes.
        std::string s;
        s.reserve(conditions.size()*32);    // profile this to find a nice default

        for (auto&& c : conditions)
        {
            s += ',';
            s += c->to_string();
        }

        auto cstr = fmt::format("(Conditions{0})");

        return fmt::format("(Def,{0},{1},{2},{3},{4},{5},{6})",
            static_cast<int>(type),
            name,
            generic_type->to_string(),
            return_type->to_string(),
            arguments_list->to_string(),
            cstr,
            body->to_string()
        );
    }
}}