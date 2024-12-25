#include "cml.h"
#include <ulib/split.h>

namespace cml
{
    Command Parse(ulib::u8string_view line)
    {
        auto split = ulib::split(line, u8"|");
        auto it = split.begin();

        Command command;
        command.name = *it;
        command.varArgs = false;

        if (line.ends_with(u8"|"))
            command.varArgs = true;

        for (++it; it != split.end(); it++)
        {
            Arg arg;
            auto word = *it;

            auto statement = ulib::split(word, u8":");
            size_t statementWords = statement.size();

            if (statementWords == 1)
            {
                arg.def = word;
                arg.type = ArgType::String;
                command.args.push_back(arg);

                continue;
            }

            if (statementWords != 2)
                throw std::runtime_error("expected {type:name}");

            auto type = statement[0];
            if (type == u8"int")
                arg.type = ArgType::Int;

            arg.name = statement[1];

            command.args.push_back(std::move(arg));
        }

        return command;
    }

    MatchResult Match(ulib::u8string_view line, const Command &command)
    {
        if (line.empty())
            return MatchResult(MatchState::DifferentName);

        auto split = ulib::split(line, u8" ");
        size_t wordsCount = split.size();

        if (wordsCount == 0)
            return MatchResult(MatchState::DifferentName);

        auto it = split.begin();
        auto fname = *it;
        if (!(fname == command.name))
            return MatchResult(MatchState::DifferentName);

        if (!command.varArgs)
            if (wordsCount - 1 != command.args.size())
                return MatchResult(MatchState::InvalidArgumentCount);

        ulib::List<ulib::u8string> params;

        size_t index = 0;
        for (++it; it != split.end(); ++it, ++index)
        {
            ulib::u8string_view param = *it;

            if (index >= command.args.size() && command.varArgs)
            {
                params.push_back(param);
            }
            else
            {
                const auto &arg = command.args[index];
                if (arg.type == ArgType::Int)
                {
                    for (char8 ch : param)
                    {
                        if (ch > '9' || ch < '0')
                            return MatchResult(MatchState::IncompatibleType);
                    }
                }

                if (!arg.def.empty())
                {
                    if (!(param == arg.def))
                        return MatchResult(MatchState::IncompatibleConstant);
                }
                else
                {
                    params.push_back(param);
                }
            }
        }

        return MatchResult(std::move(params));
    }
} // namespace cml