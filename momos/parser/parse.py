from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
from typing import TYPE_CHECKING

from comment_parser.comment_parser import extract_comments_from_str
from lark import Lark, Transformer
from lark.exceptions import UnexpectedCharacters, UnexpectedEOF, UnexpectedToken

from ..components import State, Transition, Trigger
from ..graph import StateGraph
from ..utils import Resolvable

if TYPE_CHECKING:
    from typing import List


class GrammarTransformer(Transformer):
    def state(self, items):
        options = items[1] if len(items) > 1 else {}
        return State(items[0], **options)

    def transition(self, items):
        options = items[2] if len(items) > 2 else {}
        typ = options.pop('trigger', None)
        return Transition(from_state=Resolvable.lazy(State, items[0]),
                          to_state=Resolvable.lazy(State, items[1]),
                          trigger=Trigger.of(typ, **options))

    def options(self, items):
        return dict(items)

    def key_val_pair(self, items):
        return tuple(items)

    def function(self, items):
        return [Path(items[0]), str(items[1])]

    def identifier(self, items):
        return str(items[0])

    def string(self, items):
        return str(items[0])

    def character(self, items):
        return str(items[0])

    def float(self, items):
        return float(items[0])

    def integer(self, items):
        return int(items[0])

    def boolean(self, items):
        return items[0] == 'true'


class ParseError(RuntimeError):
    def __init__(self, message: str, line: int):
        self.message = message
        self.line = line


def parse(text: str) -> StateGraph:
    grammer_path = Path(__file__).parent / 'grammar.lark'
    parser = Lark(grammer_path.read_text())
    transformer = GrammarTransformer()

    definitions = []

    for comment in extract_comments_from_str(text, mime='text/x-c'):
        comment_lines = [line.strip(' *') for line in comment.text().split('\n')]

        for lineno, line in enumerate(comment_lines):
            lineno += comment.line_number()

            try:
                ast = parser.parse(line)
                element = transformer.transform(ast)
                definitions.append(element)
            except (UnexpectedCharacters, UnexpectedToken) as ex:
                if ex.column > 1:
                    message = ex.args[0].split('at')[0]
                    raise ParseError(message=message, line=lineno)
            except UnexpectedEOF as ex:
                raise ParseError(message='Unexpected end', line=lineno)

    Resolvable.resolve_all(definitions)
    graph = StateGraph.of(definitions)
    return graph


def parse_file(file_path: Path) -> StateGraph:
    return parse(file_path.read_text())
