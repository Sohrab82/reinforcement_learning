#ifndef STATE_H
#define STATE_H
#include <iostream>
#include <vector>
#include <map>
#include <iomanip>
#include <string>
#include "action.hpp"

class State
{
public:
    string name;
    float reward; // immidiate reward of this state
    float value;  // state value
    bool terminal;
    vector<Action> actions; //list of possible actions to take from this state

    State()
    {
        name = "";
        reward = 0;
        value = 0;
        terminal = false;
    }

    // copy constructor
    State(const State &state)
    {
        reward = state.reward;
        value = state.value;
        terminal = state.terminal;
        actions = state.actions;
        name = state.name;
    }

    virtual ~State(){};
};

#endif