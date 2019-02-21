#ifndef TAPE_HPP_INCLUDED
#define TAPE_HPP_INCLUDED

template <char... Content>
struct Tape
{

};

template <typename Tape, size_t Index>
struct TapeGet;

template <char C, char... Content, size_t Index>
struct TapeGet<Tape<C, Content...>, Index>
{
    const static char result = TapeGet<Tape<Content...>, Index-1>::result;
};

template <char C, char... Content>
struct TapeGet<Tape<C, Content...>, 0>
{
    const static char result = C;
};

template <char C, typename Tape>
struct TapeConcat;

template <char C, char... Content>
struct TapeConcat<C, Tape<Content...>>
{
    using result = Tape<C, Content...>;
};

template <typename Tape, size_t Index, char NewVal>
struct TapeReplace;

template <char C, char... Content, size_t Index, char NewVal>
struct TapeReplace<Tape<C, Content...>, Index, NewVal>
{
    using result = typename TapeConcat<C, typename TapeReplace<Tape<Content...>, Index-1, NewVal>::result>::result;
};

template <char C, char... Content, char NewVal>
struct TapeReplace<Tape<C, Content...>, 0, NewVal>
{
    using result = Tape<NewVal, Content...>;
};

template <typename Tape>
struct TapeToString;

template <char... Content>
struct TapeToString<Tape<Content...>>
{
    static constexpr const char result[] = {Content...};
};

template <char... Content>
const char TapeToString<Tape<Content...>>::result[];

#endif // TAPE_HPP_INCLUDED
