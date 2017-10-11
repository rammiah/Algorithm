import java.util.Random;

public class MergeSort {
    private final static int count = 100000;
    static int[] temp = new int[count];

    private static void merge(int[] array, int left, int right, int mid) {
        int index = 0;
        int l = left, r = mid;
        while (l < mid && r < right) {
            if (array[l] < array[r]) {
                temp[index++] = array[l++];
            }else {
                temp[index++] = array[r++];
            }
        }

        while (l < mid) temp[index++] = array[l++];
        while (r < right) temp[index++] = array[r++];
        System.arraycopy(temp, 0, array, left, index);
    }

    private static void mergeSort(int[] array, int left, int right) {
        if (left >=  right) {
            return;
        }
        int mid = left + (right - left) / 2;
        mergeSort(array, left, mid);
        mergeSort(array, mid +1, right);
        merge(array, left, right, mid);
    }


    public static void main(String[] args) {
        int[] data = new int[count];

        Random rand = new Random();
        for (int i = 0; i < data.length; i++) {
            data[i] = rand.nextInt();
        }
        long start = System.currentTimeMillis();
        mergeSort(data, 0, data.length);
        System.out.println(System.currentTimeMillis() - start);
    }

}
