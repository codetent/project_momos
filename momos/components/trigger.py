from __future__ import annotations

from abc import ABC
from dataclasses import dataclass
from typing import TYPE_CHECKING

from .mode import FailureModeResolver, failure_mode

if TYPE_CHECKING:
    from .mode import FailureMode

    from typing import List, Optional, Dict

TRIGGER_IMPLS = {}


@dataclass
class Trigger(FailureModeResolver, ABC):
    """Base trigger class. All implementations must inherit this class and specify a short name.
    """
    variant: Optional[str] = None

    def __init_subclass__(cls, *, short_name: str):
        TRIGGER_IMPLS[short_name] = cls
        cls.name = short_name

    @property
    def type(self) -> str:
        typ = self.name or ''

        if typ and self.variant:
            typ += '#' + self.variant

        return typ

    @property
    def failure_modes(self) -> Dict[str, FailureMode]:
        """Get dict of all failure modes specified in impl class.
        """
        return {k: m for k, m in self._failure_modes.items() if m.possible}

    @staticmethod
    def of(name: str, **kwargs):
        """Create instance of registered impl class.
        """
        trigger_cls = TRIGGER_IMPLS[name]
        return trigger_cls(**kwargs)


@dataclass
class DefaultTrigger(Trigger, short_name='default'):
    @failure_mode(fails=False)
    def ok(self) -> List[None]:
        """Transition is triggered.
        """
        return [None]

    @failure_mode
    def no(self):
        """Transition is not triggered.
        """
        return []


@dataclass
class TimeoutTrigger(Trigger, short_name='timeout'):
    """Trigger type which is coupled with a specified timeout.
    """
    min_factor: float = 0.1
    max_factor: float = 1.9
    exceeding: bool = True

    @failure_mode(fails=False)
    def ok(self) -> int:
        """Timeout matches expected value.
        """
        return [1.0]

    @failure_mode(fails=lambda self: self.exceeding)
    def earlier(self) -> int:
        """Timeout less than expected.
        """
        return [self.min_factor]

    @failure_mode(fails=lambda self: not self.exceeding)
    def later(self) -> int:
        """Timeout greater than expected.
        """
        return [self.max_factor]


@dataclass
class ReceiveTrigger(Trigger, short_name='receive'):
    """Trigger waiting for an incoming message.
    """
    max_count: int = 3

    @failure_mode(fails=False)
    def ok(self) -> List[None]:
        """Message is received as expected.
        """
        return [1]

    @failure_mode
    def no(self):
        """No message is received.
        """
        return []

    @failure_mode(requires=lambda self: self.max_count > 1)
    def more(self):
        """More messages are received than expected.
        """
        return list(range(1, self.max_count + 1))


@dataclass
class SendTrigger(Trigger, short_name='send'):
    """Trigger waiting for a successful sent message.
    """
    max_count: int = 3
    check: bool = False

    @failure_mode(fails=False)
    def ok(self) -> List[None]:
        """Message is sent as expected.
        """
        return [1]

    @failure_mode(requires=lambda self: self.check)
    def no(self):
        """No message is sent.
        """
        return []

    @failure_mode(requires=lambda self: self.max_count > 1)
    def more(self):
        """More messages are received than expected.
        """
        return list(range(1, self.max_count + 1))
