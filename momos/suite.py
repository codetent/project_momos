from __future__ import annotations

from dataclasses import dataclass, field
from typing import Any, List

from caseconverter import pascalcase
from networkx.algorithms import all_simple_paths
from networkx.utils import pairwise

from .components import State, Transition
from .components.mode import FailureMode
from .graph import StateGraph


def get_transition(state_graph: StateGraph, from_state: State, to_state: State) -> Transition:
    for transition in state_graph.transitions:
        if transition.from_state == from_state and transition.to_state == to_state:
            return transition

    raise ValueError('transition not found')


def get_simple_paths(state_graph: StateGraph) -> List[List[Transition]]:
    graph = state_graph.graph

    end_nodes = [n for n in graph.nodes() if graph.out_degree(n) == 0 and graph.in_degree(n) > 0]
    end_nodes += graph.predecessors(state_graph.initial_state)
    paths = []

    for node in end_nodes:
        for path in all_simple_paths(graph, state_graph.initial_state, node):
            if graph.out_degree(node) > 0:
                path.append(state_graph.initial_state)

            paths.append([get_transition(state_graph, *p) for p in pairwise(path)])

    return paths


@dataclass
class TestStep:
    description: str
    transition: Transition
    mode: FailureMode

    @property
    def actions(self) -> List[Any]:
        return self.mode.results

    @classmethod
    def of(cls, transition: Transition, mode: FailureMode = None) -> TestStep:
        if mode is None:
            mode = transition.trigger.ok

        return cls(description=f'{transition.from_state.id}: {mode.description}', transition=transition, mode=mode)


@dataclass
class TestCase:
    id: str
    steps: List[TestStep] = field(default_factory=list)


@dataclass
class TestSuite:
    cases: List[TestCase] = field(default_factory=list)

    @classmethod
    def of(cls, graph: StateGraph) -> TestSuite:
        suite = cls()
        paths = get_simple_paths(graph)

        for path in paths:
            steps = [TestStep.of(transition) for transition in path]

            for i, transition in enumerate(path):
                if transition.trigger:
                    for mode in transition.trigger.failure_modes.values():
                        case = TestCase(id=f'{transition.from_state.id}_{pascalcase(mode.description)}')
                        case.steps = steps[:i]
                        case.steps.append(TestStep.of(transition, mode))

                        suite.cases.append(case)

        return suite
