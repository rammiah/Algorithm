package segtree

import (
	"fmt"
	"testing"

	"github.com/stretchr/testify/require"
)

func TestMaxTree(t *testing.T) {
	N := 100

	arr := make([]int, N)
	for i := range arr {
		arr[i] = i
	}

	maxTree := NewMaxTree(arr)

	fmt.Printf("max [1,10] = %v\n", maxTree.Max(1, 10))
	require.EqualValues(t, 10, maxTree.Max(1, 10))
}
