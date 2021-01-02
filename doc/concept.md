# Introduction

## Protocols

TODO: what

## Protocol Structures

TODO: why protocol = state machine

## State Machines

A state machine describes the behavior of a model. This concept focuses on finite state machines which are state machines containing only a finite number of reachable states.
In short, the states are known in advance and the overall amount does not change over time.

As state machines can be represented as graphs, the paper focuses only on the graphical representation called stated graphs. Such graphs consists of nodes and edges called states and transitions in this context. All the transitions connect two states with a top-down direction. Therefore, the graph is called directed graph. For simplifying the graphs, all transitions are only unidirectional. If a state shall be connected with its predecessor, a new transition have to be created. The advantage of using the graph form is that graph theory can be applied. This is a mathemathical subject covers many algorithms and formulas. It especially gained attentation in the last years in the sector of machine learning where nearly all agorithms are based on this theory. 

The connections between the states, called transitions, are coupled with an action. If the specified action occurs, one state transitions into another one. If there is no action, the system the current state remains the same. This demonstrates the storing capability of a state machine. It is also possible that a transition ends into the same state it is arisen from. This pattern is called cycle and is present in the most state machines. Cycles are problematic because they make it hard to predict the exact behavior especially regarding timing.

A collection of states and their transitions ordered by there occurence is called path. Paths describe the traversable ways through the state machine. If there are cycles, the count of all paths is infinite because it is possible that a feedback transition is traversed multiple times. A simple state machine is a special case without any loops which contains a finite amount of paths. As cycles are a common element in normal state machines, the processing is done using simple paths. This special type of path does not contain any loop and is therefore much easier to process. Additionally, omitting the loops does not change the behavior of the state graph any further because a loop in a path is only executing some actions multiple times.

## Graph Representations

TODO: dot, ...

## Graph Implementations

TODO: code behind

# Concept

## Graph Definition

## Graph Structure

## Test Structure

## Flow

