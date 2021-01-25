from dataclasses import dataclass
from pathlib import Path

from pyshould import should, should_not

from momos.utils import ExternalElement, Resolvable


def test_resolvable_resolve():
    @dataclass
    class Item:
        id: str

    @dataclass
    class Bar(Resolvable):
        item: Item

    items = [Item('foo')]
    parent = Bar(item=Resolvable.lazy(Item, 'foo'))
    items.append(parent)

    parent.item | should_not.be_an_instance_of(Item)
    Resolvable.resolve_all(items)
    parent.item | should.be_an_instance_of(Item)


def test_external_element():
    elem = ExternalElement.of('./sheep/bar.c:foo')

    elem.symbol | should.be_equal('foo')
    elem.location | should.be_equal(Path('./sheep/bar.c'))


def test_external_element_without_path():
    elem = ExternalElement.of('foo')

    elem.symbol | should.be_equal('foo')
    elem.location | should.be_none()


def test_external_element_instance():
    elem = ExternalElement.of(ExternalElement.of('foo'))

    elem.symbol | should.be_equal('foo')
    elem.location | should.be_none()
