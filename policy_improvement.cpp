#include <iostream>
#include <vector>
#include <map>
#include <iomanip>
#include <string>

#include "include/grid_world.hpp"

using namespace std;

int main()
{
    // print the defined grid
    print_board(board);
    // create grid object
    GridWorld grid_world(board);
    // load policy

    // choose grid & policy
    // actions are defined and deterministic.
    // each cell has only one action and leads to one distinct state
    //no random-ness

    // actions are defined and deterministic.
    // each cell has only one action, but might randomly lead to other states
    /* col=2, row=0:
    You have one action ("^>"), but it can equally through you up, or to the right*/
    /* col=1, row=2:
    You have one action ("^>"), but it can equally take you to the right, or to the left*/
    //grid_world.deterministic_policy(false, true);

    // at each cell, you can take actions to go left, right, up, down (if available) with equal probablities
    // each action has only one target state
    grid_world.random_policy(false);

    // print mapping of actions to states for each cell (state)
    grid_world.print_stateMap();
    //print policy
    grid_world.print_policy();
    // print reward, target states, state values for each cell
    grid_world.print_grid_info();

    bool policy_converged = false;
    int iter_no = 0;
    while (!policy_converged)
    {
        grid_world.evaluate_policy();
        policy_converged = grid_world.improve_policy();
        iter_no++;
        cout << endl
             << "Policy Iteration: " << iter_no << endl;
    }
    cout
        << endl
        << "After policy evaluation (value iteration):" << endl
        << endl;

    cout << "-----------------" << endl;
    grid_world.print_policy();
    grid_world.print_state_value();
    grid_world.print_action_values();
}
