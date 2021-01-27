from pathlib import Path

from comment_parser.comment_parser import extract_comments_from_str
from lark import Lark, Transformer

from ..components import FunctionCall, State, Transition, Trigger
from ..graph import StateGraph
from ..utils import Resolvable


class GrammarTransformer(Transformer):
    def state(self, items):
        options = items[1] if len(items) > 1 else {}
        return State(items[0], **options)

    def transition(self, items):
        options = items[2] if len(items) > 2 else {}
        trigger_opts = options.pop('trigger', {})
        return Transition(from_state=Resolvable.lazy(State, items[0]),
                          to_state=Resolvable.lazy(State, items[1]),
                          trigger=Trigger.of(trigger_opts.pop('type', None), **trigger_opts),
                          **options)

    def options(self, items):
        return dict(items)

    def key_val_pair(self, items):
        return tuple(items)

    def function_call(self, items):
        path, funcname = items[0]
        return FunctionCall(f'{path}.h', funcname, args=items[1:])

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


def parse(text: str) -> StateGraph:
    grammer_path = Path(__file__).parent / 'grammar.lark'
    parser = Lark(grammer_path.read_text())
    transformer = GrammarTransformer()

    definitions = []

    for comment in extract_comments_from_str(text, mime='text/x-c'):
        comment_lines = [line.strip(' *') for line in comment.text().split('\n')]

        for line in comment_lines:
            # with suppress(UnexpectedInput, UnexpectedEOF):
            try:
                ast = parser.parse(line)
                element = transformer.transform(ast)
                definitions.append(element)
            except Exception as ex:
                print(ex)

    Resolvable.resolve_all(definitions)
    return StateGraph.of(definitions)


def parse_file(file_path: Path) -> StateGraph:
    return parse(file_path.read_text())
