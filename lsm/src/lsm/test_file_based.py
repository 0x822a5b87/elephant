import os
from datetime import datetime

import pytest

from .compressor import StrCompressor
from .file_based import FileBasedDatabase
from .memory import MemoryDatabase, Record


@pytest.fixture
def db():
    db_name = "db"
    if os.path.exists(db_name):
        os.remove(db_name)
    return FileBasedDatabase(
        db_name,
        StrCompressor(),
        {"key": MemoryDatabase.get}
    )


@pytest.fixture
def curr_timestamp() -> int:
    curr = datetime.now()
    return int(round(curr.timestamp()))


@pytest.fixture
def record0(curr_timestamp) -> Record:
    return Record("record_0", "I'm record_0", curr_timestamp)

@pytest.fixture
def record1(curr_timestamp) -> Record:
    return Record("record_1", "I'm record_1", curr_timestamp)

def test_construct(db):
    assert db


def test_get_nothing_from_empty_db(db):
    assert db.get("record_0") is None


def test_close_then_get(db, record0, curr_timestamp):
    db.add(record0)
    assert db.get("record_0") == record0
    db = FileBasedDatabase("db", StrCompressor(), {})
    assert db.get("record_0") == record0


def test_add_then_overwrite(db, record0, record1, curr_timestamp):
    assert not db.get("record_0")
    assert not db.get("record_1")

    db = FileBasedDatabase("db", StrCompressor(), {})
    db.add(record0)
    assert db.get("record_0") == record0
    assert not db.get("record_1")
    db.add(record1)
    assert db.get("record_0") == record0
    assert db.get("record_1") == record1
