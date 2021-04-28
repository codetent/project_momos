from __future__ import annotations

from pathlib import Path
from typing import TYPE_CHECKING

from comment_parser.comment_parser import extract_comments_from_str
from lark import Lark, Transformer
from lark.exceptions import UnexpectedCharacters, UnexpectedEOF, UnexpectedToken

from ..components import State, Transition, Trigger
from ..graph import StateGraph
from ..utils import Resolvable

if TYPE_CHECKING:
    from typing import Tuple


class GrammarTransformer(Transformer):
    """Transformer for grammer that converts raw values to actual items.
    """
    def state(self, items) -> State:
        try:
            identifier, *options = items
        except ValueError:
            identifier = items[0]
            options = []

        return State(identifier, **dict(options))

    def transition(self, items) -> Transition:
        try:
            from_state, to_state, trigger = items
        except ValueError:
            from_state, to_state = items
            trigger = None

        if not from_state:
            return

        from_state = Resolvable.lazy(State, from_state)
        to_state = Resolvable.lazy(State, to_state)
        transition = Transition(from_state=from_state, to_state=to_state)

        if trigger:
            transition.triggers.append(trigger)

        return transition

    def trigger(self, items) -> Trigger:
        if len(items) > 1 and isinstance(items[1], str):
            typ, variant, *options = items
        else:
            typ, *options = items
            variant = None

        return Trigger.of(typ, variant=variant, **dict(options))

    def option(self, items):
        return items[0]

    def pair(self, items) -> Tuple:
        return tuple(items)

    def flag(self, items) -> Tuple:
        return (str(items[-1]), len(items) == 1)

    def implicit(self, items) -> None:
        return None

    def identifier(self, items) -> str:
        return str(items[0])

    def string(self, items) -> str:
        return str(items[0])

    def character(self, items) -> str:
        return str(items[0])

    def float(self, items) -> float:
        return float(items[0])

    def integer(self, items) -> int:
        return int(items[0])


class ParseError(RuntimeError):
    """Error indicating a parsing problem.
    """
    def __init__(self, message: str, line: int):
        self.message = message
        self.line = line


def parse(text: str) -> StateGraph:
    """Parse given source code text into a state graph.
    """
    grammer_path = Path(__file__).parent / 'grammar.lark'
    parser = Lark(grammer_path.read_text())
    transformer = GrammarTransformer()

    definitions = []

    for comment in extract_comments_from_str(text, mime='text/x-c'):
        comment_lines = [line.strip(' *') for line in comment.text().split('\n')]

        for lineno, line in enumerate(comment_lines):
            lineno += comment.line_number()

            if not line.startswith('@'):
                continue

            try:
                ast = parser.parse(line)
                element = transformer.transform(ast)
                definitions.append(element)
            except (UnexpectedCharacters, UnexpectedToken) as ex:
                if ex.column > 1:
                    message = ex.args[0].split('at')[0]
                    raise ParseError(message=message, line=lineno)
            except UnexpectedEOF:
                raise ParseError(message='Unexpected end', line=lineno)

    Resolvable.resolve_all(definitions)
    graph = StateGraph.of(definitions)
    return graph


def parse_file(file_path: Path) -> StateGraph:
    """Parse file at given path into a state graph.
    """
    return parse(file_path.read_text())
