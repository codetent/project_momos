from abc import ABC, abstractclassmethod
from dataclasses import dataclass, asdict
from pathlib import Path

from comment_parser import comment_parser


BUILDABLE_CLASSES = {}


def lazy_element(typ, name):
    def _resolve(elements):
        for element in elements:
            if isinstance(element, typ) and getattr(element, 'name') == name:
                return element
        
        raise ValueError('element not found')
    return _resolve


class Buildable(ABC):
    def __init_subclass__(cls, identifier):
        BUILDABLE_CLASSES[identifier] = cls
        super().__init_subclass__()

    @abstractclassmethod
    def of(cls, args):
        pass

    def resolve(self, elements):
        for key, value in asdict(self).items():
            if callable(value):
                setattr(self, key, value(elements))


@dataclass
class State(Buildable, identifier='state'):
    name: str

    @classmethod
    def of(cls, args):
        return cls(args)


@dataclass
class Condition(Buildable, identifier='condition'):
    name: str

    @classmethod
    def of(cls, args):
        name, *other = args.split()
        return cls(name)


@dataclass
class Transition(Buildable, identifier='transition'):
    from_state: State
    to_state: State
    condition: Condition
    
    @classmethod
    def of(cls, args):
        from_name, _, to_name, *other = args.split()

        return cls(
            from_state=lazy_element(State, from_name),
            to_state=lazy_element(State, to_name),
            condition=lazy_element(Condition, other[0][1:-1]) if other else None
        )


if __name__ == '__main__':
    path = Path(__file__).parent / '..' / 'src' / 'states.c'
    elements = []

    for comment in comment_parser.extract_comments(path):
        lines = [l.strip(' *') for l in comment.text().split('\n')]
        lines = [l for l in lines if l]

        for line in lines:
            if line[0] != '@':
                continue

            typ, args = line.split(' ', 1)

            element_cls = BUILDABLE_CLASSES.get(typ[1:], None)
            if element_cls:
                element = element_cls.of(args)
                elements.append(element)

    for element in elements:
        element.resolve(elements)
        print(element)
