#include <iostream>

#include "turing/machine.hpp"

int main()
{
    using machine = TuringMachine<
        1,
        1,
        State<1,
            Rule<'*', '*', 0, -1>,
            Rule<'_', '_', 0, 0>,
            Rule<'b', 'b', 1, 1>
        >
    >;

    using tape = Tape<'_', 'b', 'A', '_', '\0'>;
    using result = Run<machine, tape>;

    using str = TapeToString<result::result>;

    std::cout.write(str::result, sizeof(str::result));
    std::cout << std::endl;
    std::cout << (result::accept ? "accept" : "reject") << std::endl;

    return 0;
}
