from __future__ import annotations

from dataclasses import dataclass, field
from typing import List

from .trigger import DefaultTrigger, Trigger
from ..utils import Resolvable


@dataclass(frozen=True)
class State:
    id: str
    initial: bool = False


@dataclass
class Transition(Resolvable):
    from_state: State
    to_state: State
    triggers: List[Trigger] = field(default_factory=list)

    @property
    def default_trigger(self):
        try:
            return self.triggers[0]
        except IndexError:
            return DefaultTrigger()
