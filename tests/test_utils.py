from dataclasses import dataclass

from pyshould import should, should_not
from pytest import raises

from momos.utils import Resolvable


@dataclass
class Item:
    id: str


@dataclass
class Bar(Resolvable):
    item: Item


def test_resolvable_resolve():
    items = [Item('foo')]
    parent = Bar(item=Resolvable.lazy(Item, 'foo'))
    items.append(parent)

    parent.item | should_not.be_an_instance_of(Item)
    Resolvable.resolve_all(items)
    parent.item | should.be_an_instance_of(Item)


def test_resolvable_resolve_unknown():
    res = Bar(item=Resolvable.lazy(Item, 'foo'))

    res.item | should_not.be_an_instance_of(Item)

    with raises(ValueError):
        res.resolve([])
