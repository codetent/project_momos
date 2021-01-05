from functools import reduce, singledispatch
from operator import or_
from pathlib import Path
from typing import Set

from jinja2 import FileSystemLoader
from jinja2.environment import Environment

from .components.mode import FailureMode
from .suite import TestCase, TestStep, TestSuite
from .utils import ExternalElement


@singledispatch
def get_includes(element) -> Set:
    return set()


@get_includes.register
def suite_includes(element: TestSuite) -> Set:
    includes = reduce(or_, map(get_includes, element.cases), set())
    includes |= reduce(or_, map(get_includes, element.metadata.__dict__.values()), set())
    return includes


@get_includes.register
def case_includes(element: TestCase) -> Set:
    return reduce(or_, map(get_includes, element.steps), set())


@get_includes.register
def step_includes(element: TestStep) -> Set:
    return get_includes(element.mode)


@get_includes.register
def mode_includes(element: FailureMode) -> Set:
    try:
        return reduce(or_, map(get_includes, element.results), set())
    except TypeError:
        return get_includes(element.results)


@get_includes.register
def extelem_include(element: ExternalElement) -> Set:
    if not element.location:
        return set()
    return {element.location}


class CodeGenerator:
    def __init__(self, style='gtest') -> None:
        self.env = Environment(trim_blocks=True, lstrip_blocks=True)
        self.env.loader = FileSystemLoader(Path(__file__).parent / 'templates' / style)

    def generate(self, element: TestSuite, includes=None) -> str:
        template = self.env.get_template('suite.j2')
        includes = includes or set()
        return template.render(
            includes=get_includes(element) | includes,
            suite=element,
        )
