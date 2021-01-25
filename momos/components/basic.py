from __future__ import annotations

from dataclasses import dataclass, field

from ..utils import Resolvable
from .trigger import Trigger


@dataclass(frozen=True)
class State:
    id: str
    initial: bool = False


@dataclass
class Transition(Resolvable):
    from_state: State
    to_state: State
    trigger: Trigger = field(default_factory=Trigger.of)
