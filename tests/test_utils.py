from dataclasses import dataclass
from pathlib import Path

from pyshould import should, should_not

from momos.utils import Resolvable


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
