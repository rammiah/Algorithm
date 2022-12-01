package segtree

import "golang.org/x/exp/constraints"

func Max[T constraints.Ordered](x, y T) T {
	if x > y {
		return x
	}
	return y
}

type ZKWMaxTree struct {
	N     int
	nodes []int
}

func NewMaxTree(nums []int) *ZKWMaxTree {
	N := 1
	for N < len(nums) {
		N *= 2
	}
	tree := &ZKWMaxTree{
		N:     N,
		nodes: make([]int, N*2),
	}
	for i, v := range nums {
		tree.nodes[i+N] = v
	}
	for i := N - 1; i > 0; i-- {
		tree.nodes[i] = Max(tree.nodes[2*i], tree.nodes[2*i+1])
	}
	return tree
}

func (t *ZKWMaxTree) Update(idx int, val int) {
	idx += t.N
	t.nodes[idx] = val
	idx /= 2

	for idx > 0 {
		t.nodes[idx] = Max(t.nodes[idx*2], t.nodes[idx*2+1])
		idx /= 2
	}
}

func (t *ZKWMaxTree) Max(i, j int) int {
	i += t.N
	j += t.N
	max := t.nodes[i]
	for i <= j {
		if i&1 == 1 {
			max = Max(t.nodes[i], max)
			i++
		}
		if j&1 == 0 {
			max = Max(t.nodes[j], max)
			j--
		}
		i /= 2
		j /= 2
	}
	return max
}
