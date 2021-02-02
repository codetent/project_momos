from __future__ import annotations

from contextlib import suppress
from dataclasses import asdict
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from typing import Any, Callable, Iterable


class Resolvable:
    """Placeholder for component which can be resolved to the actual object at a later time.
    """
    resolve_field = 'id'

    @classmethod
    def lazy(cls, typ, identifier: str) -> Callable[[], Iterable[Any]]:
        """Factory which returns resolve function for given component.
        """
        def _resolve(items: Iterable[Any]):
            for item in items:
                if isinstance(item, typ) and getattr(item, cls.resolve_field) == identifier:
                    return item

            raise ValueError(f'{typ.__name__} with id "{identifier}" not found')

        return _resolve

    @staticmethod
    def resolve_all(items: Iterable[Any]) -> None:
        """Resolve all given items into real components.
        """
        for item in items:
            with suppress(AttributeError):
                item.resolve(items)

    def resolve(self, items: Iterable[Any]) -> None:
        """Resolve instance to real component.
        """
        for key, value in asdict(self).items():
            with suppress(TypeError):
                setattr(self, key, value(items))
