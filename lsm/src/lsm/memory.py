from typing import Callable, Any, Optional

from .compressor import Compressor
from .interface import Database, Record


class MemoryDatabase(Database):
    def __init__(self,
                 compressor: Compressor,
                 key_func: dict[str, Callable[..., Any]]):
        super().__init__(key_func)
        self.compressor = compressor
        self.data: dict[str, bytes] = {}

    def add(self, record: Record):
        self.data[record.key] = self.compressor.pack(record)

    def get(self, key) -> Optional[Record]:
        if key in self.data:
            return self.compressor.unpack(self.data[key])
        return None
