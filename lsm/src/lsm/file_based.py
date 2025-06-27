import os.path
from pathlib import Path
from typing import Optional, Callable, Any

from .compressor import Compressor
from .interface import Database, Record


class FileBasedDatabase(Database):
    def __init__(self,
                 file_name: str,
                 compressor: Compressor,
                 key_func:dict[str, Callable[..., Any]]):
        super().__init__(key_func)
        self._compressor = compressor
        storage = Path(file_name)
        if not storage.exists():
            storage.touch()
        self.storage = open(str(storage), "r+b")
        print(os.path.abspath(storage.name))
        self.data:dict[str, Record] = {}

        self._load()

    def add(self, record: Record):
        self.data[record.key] = record
        packed_bata = self._compressor.pack(record)
        self.storage.write(packed_bata)
        self.storage.flush()

    def get(self, key) -> Optional[Record]:
        return self.data.get(key, None)

    def _load(self):
        self.storage.seek(0)
        raw_data = self.storage.read()
        records = self._compressor.unpack_multi(raw_data)
        self.data = {record.key: record for record in records}
