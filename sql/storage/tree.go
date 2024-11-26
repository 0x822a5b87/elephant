package storage

type BTree struct {
	root uint64             // a pointer represents non-zero page number
	get  func(uint64) BNode // dereference a pointer
	new  func(uint64) BNode // allocate a new page
	del  func(uint64) BNode // deallocate a page
}
