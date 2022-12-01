package segtree

import (
	"testing"

	"github.com/stretchr/testify/require"
)

func TestSumTree(t *testing.T) {

	N := 100

	arr := make([]int, N)
	for i := range arr {
		arr[i] = i
	}

	sumTree := NewSumTree(arr)
	require.EqualValues(t, 55, sumTree.Sum(1, 10))
}
