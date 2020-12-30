from contextlib import suppress
from dataclasses import asdict, dataclass, field
from pathlib import Path
from typing import Tuple

import networkx as nx
from comment_parser.comment_parser import extract_comments
from lark import Lark, Transformer
from lark.exceptions import UnexpectedInput, UnexpectedEOF
from networkx.drawing.nx_pydot import write_dot


class Resolvable:
    resolve_field = 'id'

    @classmethod
    def lazy(cls, typ, identifier):
        def _resolve(items):
            for item in items:
                if isinstance(item, typ) and getattr(item, cls.resolve_field) == identifier:
                    return item

            raise ValueError(f'{typ.__name__} with id "{identifier}" not found')

        return _resolve

    @staticmethod
    def resolve_all(items):
        for item in items:
            with suppress(AttributeError):
                item.resolve(items)

    def resolve(self, items):
        for key, value in asdict(self).items():
            with suppress(TypeError):
                setattr(self, key, value(items))


@dataclass(frozen=True)
class State:
    id: str

    def __str__(self):
        return self.id


@dataclass
class FuncCallSpec:
    file_path: Path
    func_name: str
    args: Tuple[str] = field(default_factory=tuple)


@dataclass
class Condition:
    id: str
    func: FuncCallSpec = None


@dataclass
class Transition(Resolvable):
    from_state: State
    to_state: State
    condition: Condition = None


class StateGraph(nx.DiGraph):
    def add_state(self, state: State):
        self.add_node(state)

    def add_transition(self, transition: Transition):
        kwargs = {}

        if transition.condition:
            kwargs['label'] = transition.condition.id

        self.add_edge(transition.from_state, transition.to_state, **kwargs)

    def save(self, path: Path):
        write_dot(self, path)

    @classmethod
    def of(cls, items, **kwargs):
        graph = cls(**kwargs)

        for item in items:
            if isinstance(item, State):
                graph.add_state(item)
            elif isinstance(item, Transition):
                graph.add_transition(item)

        return graph


class GrammarTransformer(Transformer):
    def state(self, items):
        return State(items[0])

    def condition(self, items):
        return Condition(id=items[0], func=(items[1] if len(items) > 1 else None))

    def transition(self, items):
        if len(items) > 2:
            condition = Resolvable.lazy(Condition, items[2])
        else:
            condition = None

        return Transition(from_state=Resolvable.lazy(State, items[0]),
                          to_state=Resolvable.lazy(State, items[1]),
                          condition=condition)

    def identifier(self, items):
        return str(items[0])

    def callspec(self, items):
        return FuncCallSpec(file_path=Path(items[0]), func_name=self.identifier(items[1]), args=tuple(items[2:]))

    def literal(self, items):
        return self.identifier(items)


def parse_file(path: Path):
    grammer_path = Path(__file__).parent / 'grammar.lark'
    parser = Lark(grammer_path.read_text())
    transformer = GrammarTransformer()
    definitions = []

    for comment in extract_comments(source_path):
        comment_lines = [l.strip(' *') for l in comment.text().split('\n')]

        for line in comment_lines:
            with suppress(UnexpectedInput, UnexpectedEOF):
                ast = parser.parse(line)
                element = transformer.transform(ast)
                definitions.append(element)

    Resolvable.resolve_all(definitions)

    graph = StateGraph.of(definitions)
    graph.save('state.dot')


source_path = Path(__file__).parent / '..' / 'src' / 'states.c'
parse_file(source_path)
