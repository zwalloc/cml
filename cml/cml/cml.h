#pragma once

#include <ulib/list.h>
#include <ulib/string.h>
#include <optional>

namespace cml
{
    enum class ArgType
    {
        Int,
        String
    };

    struct Arg
    {
        Arg() : type(ArgType::String), name(u8"[Unnamed]") {}

        ArgType type;
        ulib::u8string name;
        ulib::u8string def;
    };

    struct Command
    {
        ulib::u8string name;
        ulib::List<Arg> args;
        bool varArgs;
    };

    enum class MatchState
    {
        Success = 0,
        DifferentName = 1,
        IncompatibleType = 2,
        InvalidArgumentCount = 3,
        IncompatibleConstant = 4
    };

    struct MatchResult
    {
        MatchResult(MatchState state_) : state(state_) {}
        MatchResult(ulib::List<ulib::u8string> &&args_) : state(MatchState::Success), args(std::move(args_)) {}

        MatchState state;
        std::optional<ulib::List<ulib::u8string>> args;
    };

    Command Parse(ulib::u8string_view line);
    MatchResult Match(ulib::u8string_view line, const Command &command);
} // namespace cml