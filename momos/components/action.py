from dataclasses import dataclass, field
from typing import Any, Tuple, Union

from ..utils import ExternalElement


@dataclass
class FunctionCall(ExternalElement):
    args: Tuple[Any] = field(default_factory=tuple)

    @classmethod
    def of(cls, definition: Union[str, 'FunctionCall']):
        if isinstance(definition, cls):
            return definition

        element = ExternalElement.of(definition)
        func, args = element.symbol.replace(' ', '')[:-1].split('(')
        return cls(location=element.location, symbol=func, args=args.split(','))
