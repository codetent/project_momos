from __future__ import annotations

from logging import INFO, basicConfig
from pathlib import Path

import click
from click import ClickException

from .generator import CodeGenerator
from .parser import ParseError, parse_file
from .suite import TestSuite

INDENT = ' ' * 4
YES = '\u2714'
NO = '\u274C'


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
        raise ClickException(f'Unknown flavor "{flavor}"')

    click.echo((Path(__file__).parent / 'libraries' / flavor).resolve())


@click.command('graph')
@click.option('-i', '--input-file', type=click.Path(exists=False), required=True, help='Input source file')
@click.option('-o', '--output-file', type=click.Path(), required=True, help='Generated graph file')
@click.option('--fmt', type=click.Choice(['dot', 'png']), default='dot', help='File format')
def graph(input_file: str, output_file: str, fmt: str) -> None:
    """Generate dot image of graph from given input file.
    """
    input_file = Path(input_file)
    output_file = Path(output_file)

    try:
        graph = parse_file(input_file)
    except ParseError as ex:
        raise ClickException(f'{input_file}:{ex.line} {ex.message}')
    else:
        graph.save(output_file, fmt=fmt)


@click.command('analyze')
@click.option('-i', '--input-file', type=click.Path(exists=False), required=True, help='Input source file')
def analyze(input_file: str) -> None:
    """Analyze specified state machine from given input file.
    """
    def check_states():
        click.echo(f'Initial state: {graph.initial_state.id}')
        click.echo(f'States: {len(graph.states)}')

        for state in graph.states:
            click.echo(f'{INDENT}{state.id}')

    def check_transitions():
        click.echo(f'Transitions: {len(graph.transitions)}')

        for transition in graph.transitions:
            click.echo(f'{INDENT}{transition.from_state.id} -> {transition.to_state.id}')

    def check_metrics():
        click.echo(f'Closed graph: {YES if graph.is_closed else NO}')
        click.echo()

        isolated_states = graph.isolated_states
        click.echo(f'Isolated states: {YES if graph.isolated_states else NO}')

        for state in isolated_states:
            click.echo(f'{INDENT}{state.id}')

        click.echo()

        sd_states = graph.single_degree_states
        click.echo(f'Single degree states: {YES if sd_states else NO}')

        for state in sd_states:
            click.echo(f'{INDENT}{state.id}')

    input_file = Path(input_file)

    try:
        graph = parse_file(input_file)
    except ParseError as ex:
        raise ClickException(f'{input_file}:{ex.line} {ex.message}')

    check_states()
    click.echo()
    check_transitions()
    click.echo()
    check_metrics()
    click.echo()


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

    try:
        graph = parse_file(input_file)
    except ParseError as ex:
        raise ClickException(f'{input_file}:{ex.line} {ex.message}')

    suite = TestSuite.of(graph)
    generator = CodeGenerator(flavor=flavor)
    text = generator.generate(suite, includes=[base_file.relative_to(output_file.parent)])
    output_file.write_text(text)


def run() -> None:
    """Main entry for cli.
    """
    basicConfig(
        level=INFO,
        format='%(message)s',
    )

    main.add_command(include)
    main.add_command(build)
    main.add_command(analyze)
    main.add_command(graph)
    main()
