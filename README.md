# reinforcement_learning
A reinforcement framework in C++

## Classes

### Action
```
string name = "";
    float value = 0.0;                           // action value
    float prob = 1.0;                            // probability that i choose this action
    vector<ActionStateTransition> target_states; // what state you will end up in
```
