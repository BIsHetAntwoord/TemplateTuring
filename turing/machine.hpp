#ifndef MACHINE_HPP_INCLUDED
#define MACHINE_HPP_INCLUDED

#include "turing/state.hpp"
#include "turing/tape.hpp"

template <size_t Initial, size_t StackPtr, typename... States>
struct TuringMachine
{
    const static size_t current_state = Initial;
    const static size_t stack_ptr = StackPtr;
};

template <typename Rule, typename Fallback>
struct SelectionIfPossible
{
    using result = Rule;
};

template <typename Fallback>
struct SelectionIfPossible<void, Fallback>
{
    using result = Fallback;
};

template <char C, typename... Rules>
struct SelectRuleFromValue
{
    using result = void;
};

template <char C, char Output, int StackMove, size_t NextState, typename... Rules>
struct SelectRuleFromValue<C, Rule<C, Output, StackMove, NextState>, Rules...>
{
    using result = Rule<C, Output, StackMove, NextState>;
};

template <char C, char Output, int StackMove, size_t NextState, typename... Rules>
struct SelectRuleFromValue<C, Rule<'*', Output, StackMove, NextState>, Rules...>
{
    using result = typename SelectionIfPossible<
                        typename SelectRuleFromValue<C, Rules...>::result,
                        Rule<C, Output == '*' ? C : Output, StackMove, NextState>
                    >::result;
};

template <char C, typename Rule, typename... Rules>
struct SelectRuleFromValue<C, Rule, Rules...>
{
    using result = typename SelectRuleFromValue<C, Rules...>::result;
};

template <size_t StackIndex, typename Tape, typename... Rules>
struct SelectRuleFromState
{
    using result = typename SelectRuleFromValue<TapeGet<Tape, StackIndex>::result, Rules...>::result;
};

template <size_t State, size_t StackIndex, typename Tape, typename... States>
struct SelectRuleFromStateList;

template <size_t CurrentState, size_t StackIndex, typename Tape, typename... Rules, typename... States>
struct SelectRuleFromStateList<CurrentState, StackIndex, Tape, State<CurrentState, Rules...>, States...>
{
    using result = typename SelectRuleFromState<StackIndex, Tape, Rules...>::result;
};

template <size_t State, size_t StackIndex, typename Tape, typename StateStruct, typename... States>
struct SelectRuleFromStateList<State, StackIndex, Tape, StateStruct, States...>
{
    using result = typename SelectRuleFromStateList<State, StackIndex, Tape, States...>::result;
};

template <typename Machine, typename Tape>
struct SelectRule;

template <size_t Initial, size_t StackPtr, typename... States, typename Tape>
struct SelectRule<TuringMachine<Initial, StackPtr, States...>, Tape>
{
    using result = typename SelectRuleFromStateList<Initial, StackPtr, Tape, States...>::result;
};

template <typename Rule, typename Machine, typename Tape>
struct RunRule;

template <char Input, char Output, int StackMove, size_t NextState, size_t OldState, size_t OldStackPtr, typename Tape, typename... States>
struct RunRule<Rule<Input, Output, StackMove, NextState>, TuringMachine<OldState, OldStackPtr, States...>, Tape>
{
    using machine = TuringMachine<NextState, OldStackPtr + StackMove, States...>;
    using tape = typename TapeReplace<Tape, OldStackPtr, Output>::result;
};

template <typename Machine, typename Tape>
struct RunStep
{
    using selected_rule = typename SelectRule<Machine, Tape>::result;
    using executed_rule = RunRule<selected_rule, Machine, Tape>;
    using machine = typename executed_rule::machine;
    using tape = typename executed_rule::tape;
};

template <typename Machine, typename Tape>
struct Run;

template <bool Done, typename Machine, typename Tape>
struct RunIf
{
    using step = RunStep<Machine, Tape>;
    using run = Run<typename step::machine, typename step::tape>;
    using result = typename run::result;
    const static bool accept = run::accept;

};

template <typename Machine, typename Tape>
struct RunIf<false, Machine, Tape>
{
    using result = Tape;
    const static bool accept = Machine::current_state == 0;
};

template <typename Machine>
struct IsEnd
{
    const static bool result = false;
};

template <size_t StackPtr, typename... States>
struct IsEnd<TuringMachine<0, StackPtr, States...>>
{
    const static bool result = true;
};

template <size_t StackPtr, typename... States>
struct IsEnd<TuringMachine<-1, StackPtr, States...>>
{
    const static bool result = true;
};

template <typename Machine, typename Tape>
struct Run
{
    using is_end = IsEnd<Machine>;
    using run = RunIf<!is_end::result, Machine, Tape>;
    using result = typename run::result;
    const static bool accept = run::accept;
};

#endif // MACHINE_HPP_INCLUDED
