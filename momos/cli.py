from __future__ import annotations

from pathlib import Path

import click

from .generator import CodeGenerator
from .parser import parse_file
from .suite import TestSuite


@click.group()
def main() -> None:
    pass


@click.command('include')
@click.argument('flavor')
def include(flavor: str) -> None:
    """Get input path for given code flavor.
    """
    path = (Path(__file__).parent / 'libraries' / flavor).resolve()

    if not path.exists():
        click.echo(f'Unknown flavor {flavor}', err=True)
        return

    click.echo((Path(__file__).parent / 'libraries' / flavor).resolve())


@click.command('graph')
@click.option('-i', '--input-file', type=click.Path(exists=False), required=True, help='Input source file')
@click.option('-o', '--output-file', type=click.Path(), required=True, help='Generated dot file')
def graph(input_file: str, output_file: str) -> None:
    """Generate dot image of graph from given input file.
    """
    input_file = Path(input_file)
    output_file = Path(output_file)

    graph = parse_file(input_file)
    graph.save(output_file)


@click.command('build')
@click.option('-i', '--input-file', type=click.Path(exists=False), required=True, help='Input source file')
@click.option('-b', '--base-file', type=click.Path(exists=False), required=True, help='Base test file')
@click.option('-o', '--output-file', type=click.Path(), required=True, help='Generated test file')
@click.option('-f', '--flavor', default='gtest', help='Flavor of generated test file')
def build(input_file: str, base_file: str, output_file: str, flavor: str) -> None:
    """Build test suite from given input file.
    """
    input_file = Path(input_file)
    base_file = Path(base_file)
    output_file = Path(output_file)

    graph = parse_file(input_file)
    suite = TestSuite.of(graph)

    generator = CodeGenerator(flavor=flavor)
    text = generator.generate(suite, includes=[base_file.relative_to(output_file.parent)])
    output_file.write_text(text)


def run() -> None:
    """Main entry for cli.
    """
    main.add_command(include)
    main.add_command(build)
    main.add_command(graph)
    main()
