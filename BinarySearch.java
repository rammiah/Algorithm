public class Solution {
    // 时间要求log(n)，显然是2次二分查找变形
    public int[] searchRange(int[] nums, int target) {
        // 首先判断不合理和不可能的情况
        if (nums == null
                || nums.length == 0
                || target < nums[0]
                || target > nums[nums.length - 1]) return new int[]{-1, -1};
        int left = 0, right = 0;
        int len = nums.length;
        int l = 0, r = len - 1;
        // 寻找小于等于target的即left
        int mid = 0;
        while (l <= r) {
            mid = (l + r) / 2;
            if (nums[mid] < target) l = mid + 1;
            else r = mid - 1;
        }
        left = l;
        l = 0;
        r = len - 1;
        while (l <= r) {
            mid = (l + r) / 2;
            if (nums[mid] > target) r = mid - 1;
            else l = mid + 1;
        }
        right = r;
        // 判断是否找到
        if (left < len && nums[left] == target) {
            return new int[] {left, right};
        }
        return new int[]{-1, -1};
    }
}
