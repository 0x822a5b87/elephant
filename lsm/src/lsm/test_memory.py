import time
from datetime import datetime

import pytest

from .compressor import StrCompressor
from .memory import MemoryDatabase, Record


@pytest.fixture
def db():
    return MemoryDatabase(
        StrCompressor(),
        {"key": MemoryDatabase.get}
    )


@pytest.fixture
def curr_timestamp() -> int:
    curr = datetime.now()
    return int(round(curr.timestamp()))

def test_construct(db):
    assert db


def test_get_nothing_from_empty_db(db):
    assert db.get("record_0") is None


def test_add_then_get(db, curr_timestamp):
    r0 = Record("record_0", "I'm record_0", curr_timestamp)
    db.add(r0)
    assert db.get("record_0") == r0


def test_add_then_overwrite(db, curr_timestamp):
    r0 = Record("record_0", "I'm record_0", curr_timestamp)
    r1 = Record("record_1", "I'm record_1", curr_timestamp)
    assert not db.get("record_0")
    assert not db.get("record_1")

    db.add(r0)
    assert db.get("record_0") == r0
    assert not db.get("record_1")
    db.add(r1)
    assert db.get("record_0") == r0
    assert db.get("record_1") == r1
