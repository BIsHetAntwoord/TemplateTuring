#ifndef RULE_HPP_INCLUDED
#define RULE_HPP_INCLUDED

template <char Input, char Output, int PtrMove, size_t Next>
struct Rule
{
    const static char input = Input;
    const static char output = Output;
    const static int ptr_move = PtrMove;
    const static size_t next_state = Next;
};

#endif // RULE_HPP_INCLUDED
