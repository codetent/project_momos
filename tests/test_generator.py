from pyshould import should
from pytest import raises

from momos.generator import CodeGenerator
from momos.suite import TestSuite

TestSuite.__test__ = False  # disable pytest collection


def test_generator_generate():
    suite = TestSuite([])
    generator = CodeGenerator()

    generator.generate(suite) | should.be_a_string()


def test_generator_invalid_flavor():
    with raises(ValueError):
        CodeGenerator(flavor='invalid')
