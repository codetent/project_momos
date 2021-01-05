from __future__ import annotations

import inspect
from functools import partial
from typing import Any, Callable


class UnboundFailureMode:
    """Failure mode of a trigger not bound to a trigger instance.
    """
    def __init__(self, generator: Callable, requires: Callable = None, fails: bool = True) -> None:
        self.generator = generator
        self.requires = requires
        self.fails = fails

    @property
    def description(self) -> str:
        """Get failure mode description.
        """
        return inspect.getdoc(self.generator).strip()

    def bind(self, instance: Any) -> FailureMode:
        """Bind failure mode to trigger instance.
        """
        return FailureMode(generator=self.generator, requires=self.requires, fails=self.fails, instance=instance)


class FailureMode(UnboundFailureMode):
    """Instance-bound failure mode.
    """
    def __init__(self, *args, instance: Any = None, **kwargs) -> None:
        super().__init__(*args, **kwargs)
        self.instance = instance

    @property
    def results(self):
        return self.generator(self.instance) or []

    @property
    def possible(self) -> bool:
        """Check if failure mode is possible for given settings.
        """
        if not callable(self.requires):
            return True
        return self.requires(self.instance)


def failure_mode(wrapped: Callable = None, *args, **kwargs) -> UnboundFailureMode:
    """Decorator for creating failure modes inside trigger classes.
    """
    if wrapped is None:
        return partial(failure_mode, *args, **kwargs)

    return UnboundFailureMode(wrapped, *args, **kwargs)
