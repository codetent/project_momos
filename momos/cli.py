from pathlib import Path

import click

from .generator import CodeGenerator
from .parser import parse_file
from .suite import TestSuite


@click.group()
def main():
    pass


@click.command('include')
@click.argument('flavor')
def include(flavor):
    print((Path(__file__).parent / 'libraries' / flavor).resolve())


@click.command('graph')
@click.option('-i', '--input-file', type=click.Path(exists=False), required=True)
@click.option('-o', '--output-file', type=click.Path(), required=True)
def graph(input_file, output_file):
    input_file = Path(input_file)
    output_file = Path(output_file)

    graph = parse_file(input_file)
    graph.save(output_file)


@click.command('build')
@click.option('-i', '--input-file', type=click.Path(exists=False), required=True)
@click.option('-b', '--base-file', type=click.Path(exists=False), required=True)
@click.option('-o', '--output-file', type=click.Path(), required=True)
def build(input_file, base_file, output_file):
    input_file = Path(input_file)
    base_file = Path(base_file)
    output_file = Path(output_file)

    graph = parse_file(input_file)
    suite = TestSuite.of(graph)

    generator = CodeGenerator()
    text = generator.generate(suite, includes=[base_file.relative_to(output_file.parent)])
    output_file.write_text(text)


def run():
    main.add_command(include)
    main.add_command(build)
    main.add_command(graph)
    main()
