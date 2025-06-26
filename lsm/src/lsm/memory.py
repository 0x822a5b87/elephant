from typing import Callable, Any, Optional

from .interface import Database, Record

class MemoryDatabase(Database):
    def __init__(self, key_func: dict[str, Callable[..., Any]]):
        super().__init__(key_func)
        self.data:dict[str, Record] = {}

    def add(self, record: Record):
        self.data[record.key] = record

    def get(self, key) -> Optional[Record]:
        if key in self.data:
            return self.data[key]
        return None
