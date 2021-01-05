from __future__ import annotations

from dataclasses import dataclass

from ..utils import Resolvable


@dataclass(frozen=True)
class State:
    id: str
    initial: bool = False


@dataclass
class Transition(Resolvable):
    from_state: State
    to_state: State
    trigger: 'Trigger' = None
