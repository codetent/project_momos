from contextlib import suppress
from dataclasses import asdict, dataclass
from pathlib import Path
from typing import Union


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


@dataclass
class ExternalElement:
    symbol: str
    location: Path = None

    @classmethod
    def of(cls, definition: Union[str, 'ExternalElement']):
        if isinstance(definition, ExternalElement):
            return definition

        try:
            path, element = definition.split(':')
        except ValueError:
            return cls(symbol=definition)
        else:
            return cls(location=Path(path), symbol=element)
