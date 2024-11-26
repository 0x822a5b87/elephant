package storage

import (
	"0x822a5b87/elephant/sql/errs"
	"bytes"
	"encoding/binary"
)

// offset list
func offsetPos(node BNode, idx uint16) uint16 {
	// the offset of the first KV pair is always zero,
	// so it is not stored in the list
	return SizeHeader + 8*node.nkeys() + 2*(idx-1)
}

func ptrPos(idx uint16) uint16 {
	return SizeHeader + 8*idx
}

// nodeLookupLE lookup less than or equal
func nodeLookupLE(node BNode, key []byte) uint16 {
	nkeys := node.nkeys()
	found := uint16(0)
	// the first key is a copy from the parent node,
	// thus it's always less than or equal to the key.
	for i := uint16(1); i < nkeys; i++ {
		cmp := bytes.Compare(node.getKey(i), key)
		// TODO why do we set the 'found' of i when the comparison is less than or equal?
		if cmp <= 0 {
			found = i
		}

		// TODO why do we break out of the loop when the comparison is greater than or equal?
		if cmp >= 0 {
			break
		}
	}
	return found
}

func leafInsert(newNode BNode, oldNode BNode, idx uint16, key, val []byte) {
	newNode.setHeaders(LeafNode, oldNode.nkeys()+1)
	nodeAppendRange(newNode, oldNode, 0, 0, idx)
}

// nodeAppendRange Copy he range from [srcOld, srcOld + numOfCopies) in oldNode
// to the range [dstNew, dstNew + numOfCopies) in newNode.
func nodeAppendRange(srcNode, dstNode BNode, srcIndex, dstIndex uint16, numOfCopies uint16) {
	errs.Assert(srcIndex+numOfCopies <= srcNode.nkeys())
	errs.Assert(dstIndex+numOfCopies <= dstNode.nkeys())
	if numOfCopies == 0 {
		// there is no need to copy
		return
	}

	// ptrs
	for i := uint16(0); i < numOfCopies; i++ {
		dstNode.setPtr(dstIndex+i, srcNode.getPtr(srcIndex+i))
	}

	// unlike pointer is a definite value, the offset is a relative value to its first kv pairs.
	// so we are going to need some trivial calculation.

	// the **offset value** of kv pairs at the position of dstIndex
	dstOffsetBegin := dstNode.getOffset(dstIndex)
	srcOffsetBegin := srcNode.getOffset(srcIndex)
	delta := dstOffsetBegin - srcOffsetBegin
	// NOTE ONE: The range is [1, n] because there is a subtle optimization in our data structure:
	// the first offset of kv pairs is always zero, and we ignore it, which means the index of offsets
	// starts from 1.
	//
	// Here is a more comprehensive illustration:
	//
	// values[0] -> values[1] -> ... values[i] ...
	//                 ↑                ↑
	//                 ↓                ↓
	//              offset[0] -> ... offset[i] ...
	//
	// Therefore, when we copy the value srcNodeOffsets[srcIndex] to dstNodeOffsets[dstIndex],
	// we are actually copying *kvs[srcIndex + 1] to *kvs[dstIndex + 1]. Correspondingly,
	// we must use the range of [1, n].
	//
	// NOTE TWO : how can we calculate the offset of destination node?
	//
	// [................. dst .........]
	//                     |
	///       |<- delta -> |
	//        |
	// [.... src ....]
	//
	// assume value of src is v, then the value of dst is (dst - delta) + v
	for i := uint16(1); i <= numOfCopies; i++ {
		offset := srcNode.getOffset(srcIndex+i) + delta
		dstNode.setOffset(dstIndex+i, offset)
	}

	begin := srcNode.kvPos(srcIndex)
	end := srcNode.kvPos(srcIndex + numOfCopies)
	copy(dstNode.data[dstNode.kvPos(dstIndex):], srcNode.data[begin:end])
}

func nodeAppendKV(dst BNode, idx uint16, ptr uint64, key, val []byte) {
	dst.setPtr(idx, ptr)
	pos := dst.kvPos(idx)
	binary.LittleEndian.PutUint16(dst.data[pos:], uint16(len(key)))
	binary.LittleEndian.PutUint16(dst.data[pos+2:], uint16(len(val)))
	copy(dst.data[pos+4:], key)
	copy(dst.data[pos+4+uint16(len(key)):], val)
	lenOfNewNode := 2*SizeKvLen + len(key) + len(val)
	dst.setOffset(idx+1, dst.getOffset(idx)+uint16(lenOfNewNode))
}
