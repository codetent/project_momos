from pathlib import Path

import click

from .generator import CodeGenerator
from .parser import parse_file
from .suite import TestSuite
from .utils import ExternalElement


@click.group()
def main():
    pass


@click.command('graph')
@click.option('-i', '--input-file', type=click.Path(), required=True)
@click.option('-o', '--output-file', type=click.Path(), required=True)
def graph(input_file, output_file):
    graph = parse_file(input_file)
    graph.save(output_file)


@click.command('build')
@click.option('-i', '--input-file', type=click.Path(), required=True)
@click.option('-o', '--output-file', type=click.Path(), required=True)
@click.option('--state-var', type=click.STRING, required=True)
@click.option('--data-type', type=click.STRING, required=True)
@click.option('--fn-init', type=click.STRING)
@click.option('--fn-update', type=click.STRING)
@click.option('--fn-close', type=click.STRING)
def build(input_file, output_file, state_var, data_type, fn_init, fn_update, fn_close):
    input_file = Path(input_file)
    output_file = Path(output_file)

    graph = parse_file(input_file)
    suite = TestSuite.of(graph,
                         state_var=ExternalElement.of(state_var),
                         data_type=ExternalElement.of(data_type),
                         fn_init=ExternalElement.of(fn_init) if fn_init else None,
                         fn_update=ExternalElement.of(fn_update) if fn_update else None,
                         fn_close=ExternalElement.of(fn_close) if fn_close else None)

    generator = CodeGenerator()
    text = generator.generate(suite, includes={input_file})
    output_file.write_text(text)


def run():
    main.add_command(build)
    main.add_command(graph)
    main()
