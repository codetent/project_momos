from operator import attrgetter
from pathlib import Path

import networkx as nx
from networkx.drawing.nx_pydot import write_dot

from wrapt import ObjectProxy

from .components import State, Transition


class StateNodeProxy(ObjectProxy):
    def __str__(self):
        return self.__wrapped__.id


class StateGraph:
    def __init__(self):
        self.graph = nx.DiGraph()
        self.states = []
        self.transitions = []

    @property
    def initial_state(self):
        return next(filter(attrgetter('initial'), self.states))

    def add_state(self, state: State):
        node_kwargs = {'shape': 'box', 'label': state.id}

        if state.initial:
            node_kwargs['shape'] = 'point'
            node_kwargs['width'] = 0.5
            node_kwargs['height'] = 0.5
            node_kwargs['label'] = ''
            node_kwargs['xlabel'] = state.id

        self.states.append(state)
        self.graph.add_node(StateNodeProxy(state), **node_kwargs)

    def add_transition(self, transition: Transition):
        self.transitions.append(transition)
        self.graph.add_edge(
            StateNodeProxy(transition.from_state),
            StateNodeProxy(transition.to_state),
            label=(transition.trigger.id or ''),
        )

    def save(self, path: Path):
        write_dot(self.graph, path)

    @classmethod
    def of(cls, items, **kwargs):
        graph = cls(**kwargs)

        for item in items:
            if isinstance(item, State):
                graph.add_state(item)
            elif isinstance(item, Transition):
                graph.add_transition(item)

        return graph
