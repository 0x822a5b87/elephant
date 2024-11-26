package storage

import (
	"0x822a5b87/elephant/sql/errs"
	"encoding/binary"
)

type BNode struct {
	data []byte // data can be dumped to the disk
}

func (node BNode) nodeType() NodeType {
	return NodeType(binary.LittleEndian.Uint16(node.data))
}

func (node BNode) nkeys() uint16 {
	return binary.LittleEndian.Uint16(node.data[SizeType:])
}

func (node BNode) setHeaders(nodeType NodeType, numOfKeys uint16) {
	binary.LittleEndian.PutUint16(node.data[0:SizeType], uint16(nodeType))
	binary.LittleEndian.PutUint16(node.data[SizeType:], numOfKeys)
}

func (node BNode) getPtr(index uint16) uint64 {
	errs.Assert(index < node.nkeys())
	pos := ptrPos(index)
	return binary.LittleEndian.Uint64(node.data[pos:])
}

func (node BNode) setPtr(index uint16, ptr uint64) {
	errs.Assert(index < node.nkeys())
	pos := ptrPos(index)
	binary.LittleEndian.PutUint64(node.data[pos:], ptr)
}

func (node BNode) getOffset(idx uint16) uint16 {
	if idx == 0 {
		return 0
	}
	return binary.LittleEndian.Uint16(node.data[offsetPos(node, idx):])
}

func (node BNode) setOffset(idx uint16, offset uint16) {
	errs.AssertWithMsg(idx > 0, "index = [%d] out of range", idx)
	binary.LittleEndian.PutUint16(node.data[offsetPos(node, idx):], offset)
}

func (node BNode) kvPos(idx uint16) uint16 {
	// beware the range of idx is less or equal, because we
	// store the size of the node at the far last position of the offset.
	// This also means the length of offsets is longer than ptrs.
	errs.Assert(idx <= node.nkeys())
	offset := node.getOffset(idx)
	return SizeHeader + node.nkeys()*(SizeOffset+SizePtr) + offset
}

func (node BNode) getKey(idx uint16) []byte {
	errs.Assert(idx < node.nkeys())
	pos := node.kvPos(idx)
	klen := binary.LittleEndian.Uint16(node.data[pos:])
	// size(klen) + size(vlen)
	return node.data[pos+SizeKvLen*2:][:klen]
}

func (node BNode) getVal(idx uint16) []byte {
	errs.Assert(idx < node.nkeys())
	position := node.kvPos(idx)
	klen := binary.LittleEndian.Uint16(node.data[position:])
	vlen := binary.LittleEndian.Uint16(node.data[position+SizeKvLen:])
	valPos := position + SizeKvLen*2 + klen
	return node.data[valPos:][:vlen]
}

// nbytes determining the size of the node.
// We store the offset to the end of the last KV pair in the offset list, which is used to determine the size of the node
func (node BNode) nbytes() uint16 {
	return node.kvPos(node.nkeys())
}
