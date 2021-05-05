#ifndef GRIDSTATE_H
#define GRIDSTATE_H

#include "state.hpp"

class GridState : public State
{
public:
    int row, col;
    GridState(int r, int c) : row(r), col(c){};
    ~GridState(){};

    // copy constructor, first calling the parent's (State) copy constructor
    GridState(const GridState &state) : State(state)
    {
        row = state.row;
        col = state.col;
    }
};

#endif