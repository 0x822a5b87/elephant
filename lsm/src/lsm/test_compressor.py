from .compressor import StrCompressor
from .interface import Record


def test_pack():
    compressor = StrCompressor(record_size=20, encode="utf-8")
    record = Record(key="abc", timestamp=123, value="xyz")
    packed_data = compressor.pack(record)
    assert packed_data == b"abc\x00123\x00xyz\x00\x00\x00\x00\x00\x00\x00\x00\x00"


def test_unpack():
    compressor = StrCompressor(record_size=20, encode="utf-8")
    packed_data = b"abc\x00123\x00xyz\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    unpacked_record = compressor.unpack(packed_data)
    print(unpacked_record)
    assert unpacked_record.key == "abc"
    assert unpacked_record.timestamp == 123
    assert unpacked_record.value == "xyz"
    assert isinstance(unpacked_record.timestamp, int)
    assert isinstance(unpacked_record.value, str)
    assert isinstance(unpacked_record.key, str)
    assert unpacked_record.timestamp == 123
