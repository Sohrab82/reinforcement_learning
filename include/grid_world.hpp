#ifndef GRID_WORLD_H
#define GRID_WORLD_H

#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>
#include <iomanip>
#include <string>
#include "grid_state.hpp"
#include "action.hpp"
#include <random>
#include <ctime>
#include <cstdlib>

using namespace std;

const int W = 4;
const int H = 3;
const float R = -4;
const float Gamma = 0.9;


// wall/out of game/no state assigned: 0
// otherwise 1
const int board[H][W] = {{1, 1, 1, 1}, {1, 0, 1, 1}, {1, 1, 1, 1}};

void print_board(const int board[][W]);

class GridWorld
{
public:
    // vector of pointers to all the state of the grid
    vector<GridState *> states;
    // the state (if any) mapped to each cell
    typedef GridState *statePtr;
    statePtr stateMap[H][W];

    // constructor
    // board[r][c] == 1: default, (walking states) getting a state
    // board[r][c] == 0: wall/out of game/no state
    GridWorld(const int board[][W]);

    // prints info about all the values, rewards, actions
    void print_grid_info();

    void print_policy();
    void print_stateMap();
    void print_action_values();
    void print_state_value();
    ~GridWorld() = default;

    // actions are defined and deterministic.
    // actions are defined and deterministic.
    // each cell has only one action, but might randomly lead to other states
    /* col=2, row=0:
    You have one action ("^>"), but it can equally through you up, or to the right*/
    /* col=1, row=2:
    You have one action ("^>"), but it can equally take you to the right, or to the left*/
    //if start_ur==true: at starting position (2, 0), policy is 50% choose up, 50% choose right
    // if windy==true: next to losing state(1, 2) the policy is to go up, but you might go up or you might go right to the losing state
    void deterministic_policy(bool start_ur, bool windy);

    // at each cell, you can take actions to go left, right, up, down (if available) with equal probablities
    // each action has only one target state
    void random_policy(bool equal_chance);

    bool improve_policy();
    void evaluate_policy();

private:
    bool has_left(int r, int c)
    {
        return (c > 0 && stateMap[r][c - 1] != nullptr);
    }

    bool has_right(int r, int c)
    {
        return (c < W - 1 && stateMap[r][c + 1] != nullptr);
    }

    bool has_up(int r, int c)
    {
        return (r > 0 && stateMap[r - 1][c] != nullptr);
    }

    bool has_down(int r, int c)
    {
        return (r < H - 1 && stateMap[r + 1][c] != nullptr);
    }

    int num_neighbors(int r, int c)
    {
        int n = 0;
        if (has_left(r, c))
            n++;
        if (has_right(r, c))
            n++;
        if (has_up(r, c))
            n++;
        if (has_down(r, c))
            n++;
        return n;
    }
};

#endif