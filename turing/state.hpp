#ifndef STATE_HPP_INCLUDED
#define STATE_HPP_INCLUDED

#include "turing/rule.hpp"
#include <cstddef>

template <size_t StateNo, typename... Rules>
struct State
{
    const static size_t state = StateNo;
};

#endif // STATE_HPP_INCLUDED
