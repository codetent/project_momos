# Concept

<!---
Add why protocol = state machine
-->

## State Machines

A state machine describes the behavior of a model. This concept focuses on finite state machines which are state machines containing only a finite number of reachable states.
In short, the states are known in advance and the overall amount does not change over time.

The connections between the states are called transitions and are coupled with an action. If the specified action occurs, one state transitions into another one. If there is no action,
the system the current state remains the same. This demonstrates the storing capability of a state machine. It is also possible that a transition ends into the same state it is arisen from.
This pattern is called cycle and is present in the most state machines. Cycles are problematic because they make it hard to predict the exact behavior especially regarding timing.

A collection of states and their transitions ordered by there occurence is called path. Paths describe the traversable ways through the state machine. If there are cycles, the count of all
paths is infinite because it is possible that a feedback transition is traversed multiple times. A simple state machine is a special case without any loops which contains a finite amount
of paths. As cycles are a common element in normal state machines, the processing is done using simple paths. This special type of path does not contain any loop.
