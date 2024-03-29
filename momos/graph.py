from __future__ import annotations

from operator import attrgetter
from pathlib import Path
from typing import TYPE_CHECKING

import networkx as nx
from networkx.algorithms import shortest_path
from networkx.drawing.nx_pydot import to_pydot
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
        self.states = []
        self.transitions = []

    @property
    def graph(self) -> nx.DiGraph:
        """Get the underlying networkx graph.
        """
        graph = nx.DiGraph()

        for state in self.states:
            graph.add_node(IdStrProxy(state), shape='box', label=state.id)

        for transition in self.transitions:
            graph.add_edge(
                IdStrProxy(transition.from_state),
                IdStrProxy(transition.to_state),
                label=' | '.join([t.type for t in transition.triggers]),
            )

        return graph

    @property
    def initial_state(self) -> Optional[State]:
        """Get initial state of the graph.
        """
        try:
            return IdStrProxy(next(filter(attrgetter('initial'), self.states)))
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
    def simple_edge_paths(self) -> Tuple[Tuple[Transition]]:
        """Get all paths without loops that are ending with an isolated state or with the initial node.
        """
        paths = list()

        for transition in self.transitions:
            path = [
                self.get_transition(*p)
                for p in pairwise(shortest_path(self.graph, self.initial_state, transition.from_state))
            ]
            path.append(transition)
            paths.append(path)

        return tuple(paths)

    @property
    def dot_graph(self) -> str:
        settings = {'fontname': 'helvetica'}
        pad = 2

        graph = self.graph.copy()
        graph.graph['node'] = settings
        graph.graph['edge'] = settings

        graph.add_node('__initial__', shape='point', width=0.2, height=0.2)
        graph.add_edge('__initial__', self.initial_state)

        # Pad labels
        for _, _, data in graph.edges.data():
            if 'label' in data and data['label']:
                data['label'] = ' ' * pad + data['label'] + ' ' * pad

        return to_pydot(graph)

    def get_transition(self, from_state: State, to_state: State) -> Transition:
        """Get transition from one state to another.
        """
        for transition in self.transitions:
            if transition.from_state == from_state and transition.to_state == to_state:
                return transition

        raise ValueError('No transition found')

    def save(self, path: Path, fmt: str = 'dot') -> None:
        """Save graph to dot file.
        """
        if fmt == 'dot':
            path.write_text(self.dot_graph.to_string())
        else:
            self.dot_graph.write(path, format=fmt)

    @classmethod
    def of(cls, items: Iterable[Union[State, Transition]]) -> StateGraph:
        """Create new state graph ot of given states and transitions.
        """
        graph = cls()

        for item in items:
            if isinstance(item, State):
                graph.states.append(item)
            elif isinstance(item, Transition):
                try:
                    transition = graph.get_transition(item.from_state, item.to_state)
                except ValueError:
                    graph.transitions.append(item)
                else:
                    transition.triggers += item.triggers

        return graph
