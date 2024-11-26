package errs

import (
	"errors"
	"fmt"
)

var ErrNodeSizeOverflow = errors.New("node size overflow")

func Assert(condition bool) {
	if !condition {
		panic("assertion failed")
	}
}

func AssertWithMsg(condition bool, format string, a ...any) {
	if !condition {
		panic(fmt.Errorf(format, a...))
	}
}
