#include "include/grid_world.hpp"

GridWorld::GridWorld(const int board[][W])
{
    // board[r][c] == 1: default, (walking states) getting a state
    // board[r][c] == 0: wall/out of game/no state
    srand(time(NULL));
    // create all the states for this board
    for (int r = 0; r < H; r++)
        for (int c = 0; c < W; c++)
        {
            if (board[r][c] == 1)
            { //default (walking states)
                GridState *s = new GridState(r, c);
                s->reward = R; // default reward
                s->value = 0.0;
                states.push_back(s);
                stateMap[r][c] = s;
            }
            else if (board[r][c] == 0)
            { // wall/out of game/no state
                stateMap[r][c] = nullptr;
                continue;
            }
            else
                cerr << "Undefined cell in the board";
        }
}

void GridWorld::print_grid_info()
{
    // prints info about all the values, rewards, actions
    cout << "Grid info: " << endl;
    for (int r = 0; r < H; r++)
    {
        for (int c = 0; c < W; c++)
        {
            cout << "------------" << endl;
            GridState *s = stateMap[r][c];
            if (s == nullptr)
                continue;
            cout << "(" << r << "," << c << "), reward= " << s->reward << ", val=" << s->value << endl;
            for (size_t a = 0; a < s->actions.size(); a++)
            {
                Action action = s->actions[a];
                cout << "    Action \"" << action.name << "\", prob= " << action.prob << ", val= " << action.value << endl;
                for (size_t k = 0; k < action.target_states.size(); k++)
                {
                    ActionStateTransition ast = action.target_states[k];
                    GridState *ts = dynamic_cast<GridState *>(ast.target_state);
                    cout << "        target: (" << ts->row << "," << ts->col << "), prob=" << ast.prob << endl;
                }
            }
        }
    }
    cout << endl;
}

void GridWorld::print_policy()
{
    cout << "Policy: " << endl;
    for (int r = 0; r < H; r++)
    {
        for (int c = 0; c < W; c++)
        {
            string s = "";
            if (stateMap[r][c] != nullptr)
            {
                for (size_t a = 0; a < stateMap[r][c]->actions.size(); a++)
                {
                    // char buffer[50];
                    // sprintf(buffer, " %s %0.2f %d |", stateMap[r][c]->actions[a].name.c_str(), stateMap[r][c]->actions[a].prob, stateMap[r][c]->actions[a].target_states.size());
                    // s += buffer;
                    if (stateMap[r][c]->actions[a].prob != 0.0)
                        s += stateMap[r][c]->actions[a].name;
                }
            }
            if (s.empty())
                s = "          |";
            cout << setw(10) << s;
        }
        cout << endl;
    }
    cout << endl;
}

void GridWorld::print_stateMap()
{
    cout << "State Reward Map:" << endl
         << "------------" << endl;
    cout << std::fixed << std::setprecision(2);
    for (int r = 0; r < H; r++)
    {
        for (int c = 0; c < W; c++)
            if (stateMap[r][c] != nullptr)
                cout << setw(6) << stateMap[r][c]->reward;
            else
                cout << setw(6) << " X";
        cout << endl;
    }
    cout << endl;
};

void GridWorld::print_action_values()
{
    cout << "----------" << endl;
    cout << "Action Value Map:" << endl;
    cout << std::fixed << std::setprecision(2);
    for (int r = 0; r < H; r++)
    {
        for (int c = 0; c < W; c++)
        {
            cout << "(" << r << "," << c << ")" << endl;
            if (stateMap[r][c] == nullptr)
                continue;
            for (size_t a = 0; a < stateMap[r][c]->actions.size(); a++)
            {
                Action action = stateMap[r][c]->actions[a];
                cout << "   Action " << action.name << ": " << action.value << endl;
            }
        }
    }
};

void GridWorld::print_state_value()
{
    cout << "State Value:" << endl;
    for (int r = 0; r < H; r++)
    {
        for (int c = 0; c < W; c++)
            if (stateMap[r][c] != NULL)
                cout << setw(6) << stateMap[r][c]->value;
            else
                cout << setw(6) << " X";
        cout << endl;
    }
};

void GridWorld::deterministic_policy(bool start_ur, bool windy)
{
    //if start_ur==true: at starting position (2, 0), policy is 50% choose up, 50% choose right
    // if windy==true: next to losing state(1, 2) the policy is to go up, but you might go up or you might go right to the losing state
    // define winning & losing states (both terminal states)
    stateMap[0][3]->reward = +1;
    stateMap[1][3]->reward = -1;
    stateMap[0][3]->terminal = true;
    stateMap[1][3]->terminal = true;

    ActionStateTransition x;
    x.prob = 1.0;

    stateMap[0][0]->actions.push_back(Action(1.0, ">"));
    x.target_state = stateMap[0][1];
    stateMap[0][0]->actions[0].target_states.push_back(x);

    stateMap[0][1]->actions.push_back(Action(1.0, ">"));
    x.target_state = stateMap[0][2];
    stateMap[0][1]->actions[0].target_states.push_back(x);

    stateMap[0][2]->actions.push_back(Action(1.0, ">"));
    x.target_state = stateMap[0][3];
    stateMap[0][2]->actions[0].target_states.push_back(x);

    stateMap[1][0]->actions.push_back(Action(1.0, "^"));
    x.target_state = stateMap[0][0];
    stateMap[1][0]->actions[0].target_states.push_back(x);

    if (windy)
    {
        /* col=1, row=2:
        You have one action ("^>"), but it can equally take you to the right or to the left*/
        Action a2(1.0, "^>");
        ActionStateTransition ast2;

        ast2.prob = 0.5;
        ast2.target_state = stateMap[0][2];
        a2.target_states.push_back(ast2);

        ast2.prob = 0.5;
        ast2.target_state = stateMap[1][3];
        a2.target_states.push_back(ast2);

        stateMap[1][2]->actions.push_back(a2);
    }
    else
    {
        stateMap[1][2]->actions.push_back(Action(1.0, "^"));
        x.target_state = stateMap[0][2];
        stateMap[1][2]->actions[0].target_states.push_back(x);
    }

    if (start_ur)
    {
        /* col=2, row=0:
        You have one action ("^>"), but it can equally through you up or to the right*/
        Action a1(1.0, "^>");
        ActionStateTransition ast1;

        ast1.prob = 0.5;
        ast1.target_state = stateMap[1][0];
        a1.target_states.push_back(ast1);

        ast1.prob = 0.5;
        ast1.target_state = stateMap[2][1];
        a1.target_states.push_back(ast1);

        stateMap[2][0]->actions.push_back(a1);
    }
    else
    {
        stateMap[2][0]->actions.push_back(Action(1.0, "^"));
        x.target_state = stateMap[1][0];
        stateMap[2][0]->actions[0].target_states.push_back(x);
    }

    stateMap[2][1]->actions.push_back(Action(1.0, ">"));
    x.target_state = stateMap[2][2];
    stateMap[2][1]->actions[0].target_states.push_back(x);

    stateMap[2][2]->actions.push_back(Action(1.0, "^"));
    x.target_state = stateMap[1][2];
    stateMap[2][2]->actions[0].target_states.push_back(x);

    stateMap[2][3]->actions.push_back(Action(1.0, "<"));
    x.target_state = stateMap[2][2];
    stateMap[2][3]->actions[0].target_states.push_back(x);
}

void GridWorld::random_policy(bool equal_chance)
{
    // each cell gets a list of possible actions to take {R , L, U, D} depending on its neighbor cells
    // equal_chance==true means every action for each cell will have equal probablity
    // otherwise, one of them gets prob=1 or the others get prob=0
    // define winning & losing states (both terminal states)
    stateMap[0][3]->reward = +1;
    stateMap[1][3]->reward = -1;
    stateMap[0][3]->terminal = true;
    stateMap[1][3]->terminal = true;

    srand((unsigned)time(0));
    float action_prob = 0.0;
    for (int r = 0; r < H; r++)
    {
        for (int c = 0; c < W; c++)
        {
            if (stateMap[r][c] == nullptr || stateMap[r][c]->terminal)
                continue;
            int n = num_neighbors(r, c);
            int chosen_action = (rand() % n); // used in case of equal_chance=false
            // cout << r << ":" << c << ":" << n << ":" << chosen_action << endl;
            int i_act = 0;
            if (has_left(r, c))
            {
                if (equal_chance)
                {
                    action_prob = 1.0 / n;
                }
                else
                {
                    if (chosen_action == i_act)
                        action_prob = 1.0;
                    else
                        action_prob = 0.0;
                }
                Action a(action_prob, "<");
                ActionStateTransition ast;
                ast.prob = 1.0;
                ast.target_state = stateMap[r][c - 1];
                a.target_states.push_back(ast);
                stateMap[r][c]->actions.push_back(a);
                i_act++;
            }
            if (has_right(r, c))
            {
                if (equal_chance)
                {
                    action_prob = 1.0 / n;
                }
                else
                {
                    if (chosen_action == i_act)
                        action_prob = 1.0;
                    else
                        action_prob = 0.0;
                }
                Action a(action_prob, ">");
                ActionStateTransition ast;
                ast.prob = 1.0;
                ast.target_state = stateMap[r][c + 1];
                a.target_states.push_back(ast);
                stateMap[r][c]->actions.push_back(a);
                i_act++;
            }
            if (has_up(r, c))
            {
                if (equal_chance)
                {
                    action_prob = 1.0 / n;
                }
                else
                {
                    if (chosen_action == i_act)
                        action_prob = 1.0;
                    else
                        action_prob = 0.0;
                }
                Action a(action_prob, "^");
                ActionStateTransition ast;
                ast.prob = 1.0;
                ast.target_state = stateMap[r - 1][c];
                a.target_states.push_back(ast);
                stateMap[r][c]->actions.push_back(a);
                i_act++;
            }
            if (has_down(r, c))
            {
                if (equal_chance)
                {
                    action_prob = 1.0 / n;
                }
                else
                {
                    if (chosen_action == i_act)
                        action_prob = 1.0;
                    else
                        action_prob = 0.0;
                }
                Action a(action_prob, "v");
                ActionStateTransition ast;
                ast.prob = 1.0;
                ast.target_state = stateMap[r + 1][c];
                a.target_states.push_back(ast);
                stateMap[r][c]->actions.push_back(a);
                i_act++;
            }
        }
    }
    // for (int r = 0; r < H; r++)
    // {
    //     for (int c = 0; c < W; c++)
    //     {
    //         if (stateMap[r][c] == nullptr)
    //             continue;
    //         int n = stateMap[r][c]->actions.size();
    //         cout << r << " " << c << " "
    //              << n << endl;
    //         if (n > 0)
    //             stateMap[r][c]->actions[rand() % n].prob = 1.0;
    //     }
    // }
    // for (int a = 0; a < stateMap[1][2]->actions.size(); a++)
    //     if (stateMap[1][2]->actions[a].name == "^")
    //     {
    //         stateMap[1][2]->actions[a].target_states[0].prob = 0.5;
    //         ActionStateTransition ast2;
    //         ast2.prob = 0.5;
    //         ast2.target_state = stateMap[1][3];
    //         stateMap[1][2]->actions[a].target_states.push_back(ast2);
    //     }
}

bool GridWorld::improve_policy()
{
    cout << "Policy Improvement:" << endl;
    bool policy_changed = false;
    for (size_t i = 0; i < states.size(); i++)
    {
        float best_action_value = -1e6;
        int best_action_idx = -1;
        int prev_action = -1;
        for (size_t j = 0; j < states[i]->actions.size(); j++)
        {
            Action *action = &states[i]->actions[j];
            if (action->value > best_action_value)
            {
                best_action_idx = j;
                best_action_value = action->value;
                if (action->prob == 1.0)
                    prev_action = j;
            }
            action->prob = 0.0;
        }
        if (best_action_idx != -1)
        {
            states[i]->actions[best_action_idx].prob = 1.0;
            if (best_action_idx != prev_action)
            {
                policy_changed = true;
            }
        }
        else if (prev_action != -1)
            states[i]->actions[prev_action].prob = 1.0;
    }
    return !policy_changed;
}

void GridWorld::evaluate_policy()
{
    cout << "Policy Evaluation:" << endl;
    float delta_vs = 10.0;
    float tol = 1e-3;
    int iter_no = 0;
    while (delta_vs > tol)
    {
        delta_vs = 0.0;
        iter_no++;
        for (size_t i = 0; i < states.size(); i++)
        {
            float old_vs = states[i]->value;
            float vs = 0;
            for (size_t j = 0; j < states[i]->actions.size(); j++)
            {
                float sigma = 0.0;
                Action action = states[i]->actions[j];
                for (size_t k = 0; k < action.target_states.size(); k++)
                {
                    ActionStateTransition ast = action.target_states[k];
                    sigma += ast.prob * (ast.target_state->reward + Gamma * ast.target_state->value);
                }
                states[i]->actions[j].value = sigma;
                vs += action.prob * sigma;
            }
            states[i]->value = vs;
            delta_vs += (vs - old_vs) * (vs - old_vs);
        }
        cout << endl
             << "Iteration: " << iter_no << " delta_vs= " << delta_vs << endl;
        print_state_value();
    }
}

void print_board(const int board[][W])
{
    cout << "Board:" << endl
         << "------------" << endl;
    for (int r = 0; r < H; r++)
    {
        for (int c = 0; c < W; c++)
            cout << setw(4) << board[r][c];
        cout << endl;
    }
    cout << endl
         << endl;
}
