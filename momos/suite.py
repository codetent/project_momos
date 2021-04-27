from __future__ import annotations

from dataclasses import dataclass
from logging import info
from typing import Any, List

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

    def __str__(self):
        return f'{self.transition.from_state.id} -> {self.transition.to_state.id} [{self.trigger.name}]; {self.mode.id}'


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
    def last_step(self):
        return self.steps[-1]

    def __hash__(self) -> int:
        return hash(self.id)

    def __eq__(self, other):
        return self.id == other.id

    def __str__(self):
        parts = [self.description]
        parts += [f'- {s}' for s in self.steps]
        return '\n'.join(parts)


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

        info('Creating test suite')

        for transitions in graph.simple_edge_paths:
            working_steps = [
                TestStep(transition, trigger=transition.default_trigger, mode=transition.default_trigger.ok)
                for transition in transitions
            ]

            for i, transition in enumerate(transitions):
                for trigger in transition.triggers or [transition.default_trigger]:
                    for mode in trigger.failure_modes.values():
                        steps = working_steps[:i] + [TestStep(transition, trigger=trigger, mode=mode)]
                        case = TestCase(steps=steps)
                        cases.add(case)

        cases = sorted(cases, key=lambda o: o.priority)

        info(f'Created {len(cases)} test cases:')
        info('')
        for case in cases:
            info(case)
            info('')

        return cls(cases)
