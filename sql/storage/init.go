package storage

import (
	"fmt"
)

type NodeType uint16

const (
	InternalNode NodeType = 1 // I'm an internal node without kv-pairs
	LeafNode     NodeType = 2 // I'm a leaf node with kv-pairs

	SizeType   = 2
	SizeNKeys  = 2
	SizeHeader = SizeType + SizeNKeys // len(type) + len(nkeys)
	SizePtr    = 8
	SizeOffset = 2
	SizeKvLen  = 2 // size of klen or vlen
	PageSize   = 4096
	MaxKeySize = 1000
	MaxValSize = 3000
)

func init() {
	// the value represents the largest size of a node with single kv pair.
	// a node with single kv pair has the following configuration:
	// Type -> NKeys -> Ptr -> **SizeOfNode** -> [klen, vlen, key, value]
	node1max := SizeType + SizeNKeys + SizePtr + SizeOffset + SizeOffset + SizeKvLen + MaxKeySize + MaxValSize
	fmt.Printf("node max size: %d\n", node1max)
}
