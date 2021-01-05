from __future__ import annotations

from abc import ABC
from dataclasses import dataclass
from pathlib import Path
from typing import Optional

from .action import FunctionCall
from .mode import UnboundFailureMode, failure_mode

TRIGGER_IMPLS = {}


@dataclass
class Trigger(ABC):
    def __new__(cls, *args, **kwargs):
        instance = super().__new__(cls)
        instance._failure_modes = {}

        for key, value in cls.__dict__.items():
            if isinstance(value, UnboundFailureMode):
                bound = value.bind(instance)
                setattr(instance, key, bound)
                instance._failure_modes[key] = bound

        return instance

    def __init_subclass__(cls, *, short_name: Optional[str]):
        TRIGGER_IMPLS[short_name] = cls
        cls.id = short_name

    @property
    def failure_modes(self):
        return {k: m for k, m in self._failure_modes.items() if m.possible}

    @staticmethod
    def of(name: Optional[str], **kwargs):
        trigger_cls = TRIGGER_IMPLS[name]
        return trigger_cls(**kwargs)


@dataclass
class DummyTrigger(Trigger, short_name=None):
    @failure_mode(fails=False)
    def ok(self) -> None:
        """Do nothing.
        """
        return None


@dataclass
class TimeoutTrigger(Trigger, short_name='timeout'):
    value: int

    def _create_call(self, value: int) -> FunctionCall:
        return FunctionCall(symbol='sleep', location=Path('unistd.h'), args=[value])

    @failure_mode(fails=False)
    def ok(self) -> int:
        """Timeout equals expected value.
        """
        return [self._create_call(self.value)]

    @failure_mode
    def earlier(self) -> int:
        """Timeout less than expected.
        """
        return [self._create_call(round(self.value * 0.1))]

    @failure_mode(fails=False)
    def later(self) -> int:
        """Timeout greater than expected.
        """
        return [self._create_call(round(self.value * 1.9))]


@dataclass
class ReceiveTrigger(Trigger, short_name='receive'):
    builder: FunctionCall
    count: int = 1
    count_sensitive: bool = True

    def __post_init__(self):
        self.builder = FunctionCall.of(self.builder)

    @failure_mode(fails=False)
    def ok(self):
        """Expected is received.
        """
        return [self.builder]

    @failure_mode
    def no(self):
        """No message is received.
        """
        return None

    @failure_mode(requires=lambda self: self.count_sensitive)
    def more(self):
        """More messages are received than expected.
        """
        return [self.builder] * round(self.count * 1.5)

    @failure_mode(requires=lambda self: self.count_sensitive and self.count > 1)
    def less(self):
        """Less messages are received than expected.
        """
        return [self.builder] * round(self.count * 0.4)


@dataclass
class SendTrigger(ReceiveTrigger, short_name='ttransmit'):
    count: int = 1

    @failure_mode(fails=False)
    def ok(self):
        """Expected is sent.
        """
        return [self.message] * self.count

    @failure_mode
    def no(self):
        """No message is sent.
        """
        return super().no()

    @failure_mode
    def more(self):
        """More messages are sent than expected.
        """
        return super().more()

    @failure_mode(requires=lambda self: self.count > 1)
    def less(self):
        """Less messages are sent than expected.
        """
        return super().less()

    @failure_mode
    def part_of(self):
        """Malformed message is sent.
        """
        return super().part_of()

    @failure_mode
    def other_than(self):
        """Unexpected message is sent.
        """
        return super().other_than()
