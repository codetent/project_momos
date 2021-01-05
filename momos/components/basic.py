from __future__ import annotations

from dataclasses import dataclass
from typing import TYPE_CHECKING

from ..utils import Resolvable

if TYPE_CHECKING:
    from .trigger import Trigger


@dataclass(frozen=True)
class State:
    id: str
    initial: bool = False


@dataclass
class Transition(Resolvable):
    from_state: State
    to_state: State
    trigger: Trigger = None
