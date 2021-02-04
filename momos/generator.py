from __future__ import annotations

from pathlib import Path
from typing import TYPE_CHECKING

from caseconverter import pascalcase
from jinja2 import FileSystemLoader
from jinja2.environment import Environment

from .suite import TestSuite

if TYPE_CHECKING:
    from typing import Iterable, Optional


class CodeGenerator:
    """Generate test source code out of given test suite.
    """
    def __init__(self, flavor: str = 'gtest') -> None:
        """
        Args:
            flavor (str, optional): Code flavor. Defaults to 'gtest'.
        """
        path = Path(__file__).parent / 'templates' / flavor

        if not path.exists():
            raise ValueError(f'Unknown flavor {flavor}')

        self.env = Environment(trim_blocks=True, lstrip_blocks=True)
        self.env.loader = FileSystemLoader(path)
        self.env.filters['pascalcase'] = pascalcase

    def generate(self, element: TestSuite, includes: Optional[Iterable] = None) -> str:
        """Generate code out of given test suite and add given includes.

        Args:
            element (TestSuite): Test suite containing test cases for generation.
            includes (Optional[Iterable], optional): List of additional files to include. Defaults to None.

        Returns:
            str: Generated source code
        """
        template = self.env.get_template('suite.j2')
        includes = set(includes) if includes else set()
        return template.render(
            includes=includes,
            suite=element,
        )
