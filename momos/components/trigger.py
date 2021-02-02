from __future__ import annotations

from abc import ABC
from dataclasses import dataclass
from typing import Optional

from .mode import FailureModeResolver, failure_mode

TRIGGER_IMPLS = {}


@dataclass
class Trigger(FailureModeResolver, ABC):
    def __init_subclass__(cls, *, short_name: Optional[str]):
        TRIGGER_IMPLS[short_name] = cls
        cls.id = short_name

    @property
    def failure_modes(self):
        return {k: m for k, m in self._failure_modes.items() if m.possible}

    @staticmethod
    def of(name: Optional[str] = None, **kwargs):
        trigger_cls = TRIGGER_IMPLS[name]
        return trigger_cls(**kwargs)


@dataclass
class DummyTrigger(Trigger, short_name=None):
    @failure_mode(fails=False)
    def ok(self) -> None:
        """Do nothing.
        """
        return []


@dataclass
class TimeoutTrigger(Trigger, short_name='timeout'):
    value: int

    @failure_mode(fails=False)
    def ok(self) -> int:
        """Timeout equals expected value.
        """
        return [self.value]

    @failure_mode
    def earlier(self) -> int:
        """Timeout less than expected.
        """
        return [self.value * 0.1]

    @failure_mode(fails=False)
    def later(self) -> int:
        """Timeout greater than expected.
        """
        return [self.value * 1.9]


@dataclass
class ReceiveTrigger(Trigger, short_name='receive'):
    count: int = 1
    count_sensitive: bool = True

    @failure_mode(fails=False)
    def ok(self):
        """Expected is received.
        """
        return [None]

    @failure_mode
    def no(self):
        """No message is received.
        """
        return []

    @failure_mode(requires=lambda self: self.count_sensitive)
    def more(self):
        """More messages are received than expected.
        """
        return [None] * round(self.count * 1.5)

    @failure_mode(requires=lambda self: self.count_sensitive and self.count > 1)
    def less(self):
        """Less messages are received than expected.
        """
        return [None] * round(self.count * 0.4)


@dataclass
class SendTrigger(Trigger, short_name='transmit'):
    ignore_fail: bool = False

    @failure_mode(fails=False)
    def ok(self):
        """Expected is sent.
        """
        return [None]

    @failure_mode(requires=lambda self: self.ignore_fail)
    def no(self):
        """No message is sent.
        """
        return []
