from __future__ import annotations

from dataclasses import dataclass
from typing import Any, List, Optional

from .components import Transition
from .components.mode import FailureMode
from .components.trigger import Trigger
from .graph import StateGraph


@dataclass
class TestStep:
    """Step of a test case failing with the failure mode set.
    """
    transition: Transition
    trigger: Trigger
    mode: FailureMode

    @property
    def id(self) -> str:
        parts = [
            self.transition.from_state.id,
            self.transition.to_state.id,
            self.trigger.name,
        ]

        if self.trigger.variant:
            parts.append(self.trigger.variant)

        return '__'.join(parts + [self.mode.id])

    @property
    def fails(self) -> bool:
        return self.mode.fails

    @property
    def description(self) -> str:
        """Description of test step.
        """
        return f'{self.transition.from_state.id}: {self.mode.description}'

    @property
    def arguments(self) -> List[Any]:
        """Arguments for step calls.
        """
        return self.mode.arguments


@dataclass
class TestCase:
    """Test case consisting out of several test steps.
    """
    steps: List[TestStep]

    @property
    def id(self):
        return self.last_step.id

    @property
    def description(self):
        return self.last_step.description

    @property
    def priority(self):
        return len(self.steps)

    @property
    def fails(self) -> bool:
        return self.last_step.fails

    @property
    def last_step(self):
        return self.steps[-1]

    def __hash__(self) -> int:
        return hash(self.id)

    def __eq__(self, other):
        return self.id == other.id


@dataclass
class TestSuite:
    """Test suite consisting out of several test cases.
    """
    cases: List[TestCase]

    @classmethod
    def of(cls, graph: StateGraph) -> TestSuite:
        """Create new test suite out of given state graph.
        """
        cases = set()

        for transitions in graph.simple_edge_paths:
            working_steps = [
                TestStep(transition, trigger=transition.default_trigger, mode=transition.default_trigger.ok)
                for transition in transitions
            ]

            for i, transition in enumerate(transitions):
                case = TestCase(steps=working_steps[:(i + 1)])
                cases.add(case)

                for trigger in transition.triggers:
                    for mode in trigger.failure_modes.values():
                        steps = working_steps[:i] + [TestStep(transition, trigger=trigger, mode=mode)]
                        case = TestCase(steps=steps)
                        cases.add(case)

        cases = sorted(cases, key=lambda o: o.priority)

        for case in cases:
            print(case.id)

        return cls(cases)
