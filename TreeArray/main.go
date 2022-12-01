package main

import (
	"fmt"
	"math/rand"

	"golang.org/x/exp/constraints"
)

type Number interface {
	constraints.Integer | constraints.Float
}

func Max[T constraints.Ordered](x, y T) T {
	if x > y {
		return x
	}
	return y
}

func Min[T constraints.Ordered](x, y T) T {
	if x < y {
		return x
	}
	return y
}

func lowbit(x int) int {
	return x & -x
}

// TreeArray 树状数组，解决O(logn)时间区间求和问题, https://oi-wiki.org/ds/fenwick/
type TreeArray[T Number] []T

func New[T Number](n int) TreeArray[T] {
	return make(TreeArray[T], n+1)
}

// Update 更新idx节点处的值，参数delta为变化值
func (t TreeArray[T]) Update(idx int, delta T) {
	// 参数是0-index的，需要转换到1开始
	idx++
	for idx < len(t) {
		t[idx] += delta
		// 持续向右走，找到包含自己的下标
		idx += lowbit(idx)
	}
}

// Sum 求和[0,idx)的数组元素和
func (t TreeArray[T]) Sum(idx int) T {
	var result T
	// 前闭后开区间不需要特意+1了

	for idx > 0 {
		result += t[idx]
		// 持续向左走，找到自己包含的下标
		idx -= lowbit(idx)
	}

	return result
}

// SumArray 原始数组求和
func SumArray[T Number](arr []T, from, to int) T {
	var result T
	for i := from; i < to; i++ {
		result += arr[i]
	}
	return result
}

func main() {
	arr := make([]int, 100)
	t := New[int](len(arr))

	for i := range arr {
		arr[i] = rand.Intn(100)
		t.Update(i, arr[i])
	}

	for i := 0; i < 10; i++ {
		from := rand.Intn(10)
		to := Max(rand.Intn(100), from)
		fmt.Printf("tree sum %v, expect %v\n", t.Sum(to)-t.Sum(from), SumArray(arr, from, to))
	}
}
