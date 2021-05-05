#ifndef ACTION_H
#define ACTION_H
#include <iostream>
#include <vector>
#include <map>
#include <iomanip>
#include <string>
#include "state.hpp"
using namespace std;

class State;

class ActionStateTransition
{
public:
    float prob = 1.0;
    State *target_state;
};

class Action
{
public:
    float prob; // probability that i choose this action
    string name;
    float value = 0.0;                           // action value
    vector<ActionStateTransition> target_states; // what state you will end up in
    // Action()
    // {
    //     name = "";
    //     prob = 0.0;
    //     value = 0.0;
    // };
    Action(float p, string n) : prob(p), name(n){};
};

#endif