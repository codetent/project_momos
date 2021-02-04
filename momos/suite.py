from __future__ import annotations

from dataclasses import dataclass
from typing import Any, List

from .components import Transition
from .components.mode import FailureMode
from .graph import StateGraph


@dataclass
class TestStep:
    """Step of a test case failing with the failure mode set.
    """
    transition: Transition
    mode: FailureMode

    @property
    def description(self) -> str:
        """Description of test step.
        """
        return f'{self.transition.from_state.id}: {self.mode.description}'

    @property
    def arguments(self) -> List[Any]:
        """Arguments for step calls.
        """
        return self.mode.results


@dataclass
class TestCase:
    """Test case consisting out of several test steps.
    """
    mode: FailureMode
    steps: List[TestStep]


@dataclass
class TestSuite:
    """Test suite consisting out of several test cases.
    """
    cases: List[TestCase]

    @classmethod
    def of(cls, graph: StateGraph) -> TestSuite:
        """Create new test suite out of given state graph.
        """
        cases = []

        for transitions in graph.simple_paths:
            working_steps = [TestStep(transition, transition.trigger.ok) for transition in transitions]

            for i, transition in enumerate(transitions):
                for mode in transition.trigger.failure_modes.values():
                    steps = working_steps[:i] + [TestStep(transition, mode)]
                    case = TestCase(mode=mode, steps=steps)
                    cases.append(case)

        return cls(cases)
