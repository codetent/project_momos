from __future__ import annotations

import inspect
from functools import partial
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from typing import Any, Callable, List


class UnboundFailureMode:
    """Failure mode of a trigger not bound to a trigger instance.
    """
    def __init__(self, generator: Callable, requires: Callable = None, fails: bool = True) -> None:
        self.generator = generator
        self.requires = requires
        self._fails = fails

    @property
    def id(self) -> str:
        return self.generator.__name__

    @property
    def description(self) -> str:
        """Get failure mode description.
        """
        return inspect.getdoc(self.generator).strip()

    def _bind(self, instance: Any) -> FailureMode:
        """Bind failure mode to trigger instance.
        """
        return FailureMode(generator=self.generator, requires=self.requires, fails=self._fails, instance=instance)


class FailureMode(UnboundFailureMode):
    """Instance-bound failure mode.
    """
    def __init__(self, *args, instance: Any = None, **kwargs) -> None:
        super().__init__(*args, **kwargs)
        self.instance = instance

    @property
    def arguments(self) -> List[Any]:
        """Get arguments returned by failure mode.
        """
        return self.generator(self.instance) or []

    @property
    def possible(self) -> bool:
        """Check if failure mode is possible for given settings.
        """
        if not callable(self.requires):
            return True
        return self.requires(self.instance)

    @property
    def fails(self) -> bool:
        if not callable(self._fails):
            return self._fails
        return self._fails(self.instance)


class FailureModeResolver:
    """Mixin that resolves specified failure modes.
    """
    def __new__(cls, *args, **kwargs):
        instance = super().__new__(cls)
        instance._failure_modes = {}

        for key, value in cls.__dict__.items():
            if isinstance(value, UnboundFailureMode):
                bound = value._bind(instance)
                setattr(instance, key, bound)
                instance._failure_modes[key] = bound

        return instance


def failure_mode(wrapped: Callable = None, *args, **kwargs) -> UnboundFailureMode:
    """Decorator for creating failure modes inside trigger classes.
    """
    if wrapped is None:
        return partial(failure_mode, *args, **kwargs)

    return UnboundFailureMode(wrapped, *args, **kwargs)
