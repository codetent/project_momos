from __future__ import annotations

from operator import attrgetter
from pathlib import Path
from typing import TYPE_CHECKING

import networkx as nx
from networkx.algorithms import all_simple_paths
from networkx.drawing.nx_pydot import write_dot
from networkx.utils import pairwise

from wrapt import ObjectProxy

from .components import State, Transition

if TYPE_CHECKING:
    from typing import Optional, Tuple, Iterable, Union


class IdStrProxy(ObjectProxy):
    """Proxy that shows the id of the wrapped when stringified.
    """
    def __str__(self) -> str:
        return self.__wrapped__.id


class StateGraph:
    """Graph consisting of states and transitions.
    """
    def __init__(self) -> None:
        self.graph = nx.DiGraph()
        self.states = []
        self.transitions = []

    @property
    def initial_state(self) -> Optional[State]:
        """Get initial state of the graph.
        """
        try:
            return next(filter(attrgetter('initial'), self.states))
        except StopIteration:
            return None

    @property
    def isolated_states(self) -> Tuple[State]:
        """Get states without any connection.
        """
        return tuple(nx.isolates(self.graph))

    @property
    def single_degree_states(self) -> Tuple[State]:
        """Get states with only a single exiting transitions.
        """
        return tuple(s for s in self.states if self.graph.out_degree(s) == 1)

    @property
    def is_closed(self) -> bool:
        """Check if a graph is closed (never ends).
        """
        return len(list(self.graph.predecessors(self.initial_state))) > 0

    @property
    def simple_paths(self) -> Tuple[Tuple[Transition]]:
        """Get all paths without loops that are ending with an isolated state or with the initial node.
        """
        end_nodes = [n for n in self.graph.nodes() if self.graph.out_degree(n) == 0 and self.graph.in_degree(n) > 0]
        end_nodes += self.graph.predecessors(self.initial_state)
        paths = []

        for node in end_nodes:
            for path in all_simple_paths(self.graph, self.initial_state, node):
                if self.graph.out_degree(node) > 0:
                    path.append(self.initial_state)

                paths.append(tuple(self.get_transition(*p) for p in pairwise(path)))

        return tuple(paths)

    def add_state(self, state: State) -> None:
        """Add given state to graph.
        """
        node_kwargs = {'shape': 'box', 'label': state.id}

        if state.initial:
            node_kwargs['shape'] = 'point'
            node_kwargs['width'] = 0.5
            node_kwargs['height'] = 0.5
            node_kwargs['label'] = ''
            node_kwargs['xlabel'] = state.id

        self.states.append(state)
        self.graph.add_node(IdStrProxy(state), **node_kwargs)

    def add_transition(self, transition: Transition) -> None:
        """Add given transition to graph.
        """
        self.transitions.append(transition)
        self.graph.add_edge(
            IdStrProxy(transition.from_state),
            IdStrProxy(transition.to_state),
            label=(transition.trigger.id or ''),
        )

    def get_transition(self, from_state: State, to_state: State) -> Transition:
        """Get transition from one state to another.
        """
        for transition in self.transitions:
            if transition.from_state == from_state and transition.to_state == to_state:
                return transition

        raise ValueError('Transition not found')

    def save(self, path: Path) -> None:
        """Save graph to dot file.
        """
        write_dot(self.graph, path)

    @classmethod
    def of(cls, items: Iterable[Union[State, Transition]]) -> StateGraph:
        """Create new state graph ot of given states and transitions.
        """
        graph = cls()

        for item in items:
            if isinstance(item, State):
                graph.add_state(item)
            elif isinstance(item, Transition):
                graph.add_transition(item)

        return graph
