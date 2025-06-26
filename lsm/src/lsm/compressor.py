from .interface import Record


class Compressor:
    def pack(self, record: Record) -> bytes:
        """pack the data"""

    def unpack(self, binary_data: bytes) -> Record:
        """pack the data"""


class StrCompressor(Compressor):
    def __init__(self,
                 record_size: int = 255,
                 encode: str = "utf-8"):
        self.record_size = record_size
        self.encode = encode

    def pack(self, record: Record) -> bytes:
        result = f"{record.key}\0{record.timestamp}\0{record.value}"
        data = bytes(result, self.encode)
        if len(data) < self.record_size:
            data += b'\0' * (self.record_size - len(data))
        return data

    def unpack(self, binary_data: bytes) -> Record:
        # we use length-fixed bytes to store data
        assert len(binary_data) == self.record_size
        result = str(binary_data, self.encode)
        fields = [field for field in result.split("\0") if field]
        assert len(fields) == 3, f"malformed compressed record: {result}"
        key, timestamp, value = fields
        assert timestamp.isdigit(), f"malformed timestamp: {result}"
        return Record(key, value, int(timestamp))

    def pack_multi(self, records: list[Record]) -> bytes:
        result = b""
        for record_bytes in [self.pack(record) for record in records]:
            result += record_bytes
        return result

    def unpack_multi(self, binary_data: bytes) -> list[Record]:
        bytes_list = [binary_data[index: index + self.record_size]
                      for index
                      in range(0, len(binary_data), self.record_size)]

        return [self.unpack(bytes_data) for bytes_data in bytes_list]