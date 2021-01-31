from pathlib import Path

from jinja2 import FileSystemLoader
from jinja2.environment import Environment

from .suite import TestSuite


class CodeGenerator:
    def __init__(self, style='gtest') -> None:
        self.env = Environment(trim_blocks=True, lstrip_blocks=True)
        self.env.loader = FileSystemLoader(Path(__file__).parent / 'templates' / style)

    def generate(self, element: TestSuite, includes=None) -> str:
        template = self.env.get_template('suite.j2')
        includes = set(includes) if includes else set()
        return template.render(
            includes=includes,
            suite=element,
        )
