# reinforcement_learning
reinforcement learning with C++

## Classes

### ActionStateTransition (action.hpp)

If a certain action is taken, what will be the target state and with what probablity.
```
float prob;
State *target_state; 

```

### Action (action.hpp)
```
string name;
float value;                           // action value
float prob;                            // probability that i choose this action
vector<ActionStateTransition> target_states; // what state you will end up in
```

### State (state.hpp)
```
string name;
float reward = 0; // immidiate reward of this state
float value = 0;  // state value
bool terminal = false;
vector<Action> actions; //list of possible actions to take from this state
```

