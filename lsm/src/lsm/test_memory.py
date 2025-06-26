import time

import pytest

from .memory import MemoryDatabase, Record


@pytest.fixture
def db():
    return MemoryDatabase({"key": MemoryDatabase.get})


def test_construct(db):
    assert db


def test_get_nothing_from_empty_db(db):
    assert db.get("record_0") is None


def test_add_then_get(db):
    r0 = Record("record_0", "I'm record_0", time.time())
    db.add(r0)
    assert db.get("record_0") == r0


def test_add_then_overwrite(db):
    r0 = Record("record_0", "I'm record_0", time.time())
    r1 = Record("record_1", "I'm record_1", time.time())
    assert not db.get("record_0")
    assert not db.get("record_1")

    db.add(r0)
    assert db.get("record_0") == r0
    assert not db.get("record_1")
    db.add(r1)
    assert db.get("record_0") == r0
    assert db.get("record_1") == r1
