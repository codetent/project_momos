from pathlib import Path
from shutil import copy
from tempfile import TemporaryDirectory

from pyshould import should, should_not

from momos import cli

EXAMPLE_PATH = Path(__file__).parent / '..' / 'examples' / 'basic'


def test_cli_include(capsys):
    try:
        cli.include.main(['gtest'])
    except SystemExit as ex:
        ex.code | should.be_equal(0)

    output = capsys.readouterr()
    output.out | should_not.be_empty()


def test_cli_include_invalid(capsys):
    try:
        cli.include.main(['invalid'])
    except SystemExit as ex:
        ex.code | should.be_equal(1)

    output = capsys.readouterr()
    output.err.strip() | should.be_equal('Error: Unknown flavor "invalid"')


def test_cli_graph():
    with TemporaryDirectory() as tmp:
        out_path = Path(tmp) / 'graph.png'

        try:
            cli.graph.main([
                '--input-file',
                str(EXAMPLE_PATH / 'src' / 'states.c'),
                '--output-file',
                str(out_path),
            ])
        except SystemExit as ex:
            ex.code | should.be_equal(0)

        out_path.exists() | should.be_true()


def test_cli_analyze(capsys):
    try:
        cli.analyze.main([
            '--input-file',
            str(EXAMPLE_PATH / 'src' / 'states.c'),
        ])
    except SystemExit as ex:
        ex.code | should.be_equal(0)

    output = capsys.readouterr()
    stdout = output.out.strip()

    stdout | should.contain_the_substring('Initial state: ')
    stdout | should.contain_the_substring('States: ')
    stdout | should.contain_the_substring('Transitions: ')
    stdout | should.contain_the_substring('Closed graph: ')
    stdout | should.contain_the_substring('Isolated states: ')
    stdout | should.contain_the_substring('Single degree states: ')


def test_cli_build():
    with TemporaryDirectory() as tmp:
        out_path = Path(tmp) / 'test_states.cpp'
        base_path = Path(tmp) / 'base_states.cpp'

        copy(EXAMPLE_PATH / 'test' / 'base_states.cpp', base_path)

        try:
            cli.build.main([
                '--input-file',
                str(EXAMPLE_PATH / 'src' / 'states.c'),
                '--base-file',
                str(base_path),
                '--output-file',
                str(out_path),
            ])
        except SystemExit as ex:
            ex.code | should.be_equal(0)

        out_path.exists() | should.be_true()
