package segtree

type ZKWSumTree struct {
	N     int
	nodes []int
}

func NewSumTree(nums []int) *ZKWSumTree {
	N := 1
	for N < len(nums) {
		N *= 2
	}
	tree := &ZKWSumTree{
		N:     N,
		nodes: make([]int, N*2),
	}
	for i, v := range nums {
		tree.nodes[i+N] = v
	}
	for i := N - 1; i > 0; i-- {
		tree.nodes[i] += tree.nodes[2*i] + tree.nodes[2*i+1]
	}
	return tree
}

func (t *ZKWSumTree) Update(idx int, val int) {
	idx += t.N
	diff := val - t.nodes[idx]
	for idx > 0 {
		t.nodes[idx] += diff
		idx /= 2
	}
}

func (t *ZKWSumTree) Sum(i, j int) int {
	i += t.N
	j += t.N
	sum := 0
	for i <= j {
		if i&1 == 1 {
			sum += t.nodes[i]
			i++
		}
		if j&1 == 0 {
			sum += t.nodes[j]
			j--
		}
		i /= 2
		j /= 2
	}
	return sum
}
