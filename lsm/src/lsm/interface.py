from typing import Callable, Any, Optional


class Record:
    def __init__(self, key: str, value: Any, timestamp: int):
        self.key = key
        self.value = value
        self.timestamp = timestamp

    def __eq__(self, other):
        return (self.key == other.key
                and self.value == other.value
                and self.timestamp == other.timestamp)


class Database:
    def __init__(self, key_func: dict[str, Callable[..., Any]]):
        self.key_func = key_func

    def add(self, record: Record):
        """store the record in the database"""
        raise NotImplementedError("not implemented")

    def get(self, key) -> Optional[Record]:
        """get the record from the database"""
        raise NotImplementedError("not implemented")
