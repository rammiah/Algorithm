import java.util.Arrays;
import java.util.Random;

public class ShellSort {
    private static void shellSort(int[] array) {
        int len = array.length;
        int n = 1;
        while (n < len / 3) n = n * 3 + 1;
        int t = 0;
        while (n >= 1) {
            for (int i = n; i < len; i++) {
                for (int index = i; index >= n && array[index] < array[index - n]; index-=n) {
                    t = array[index];
                    array[index] = array[index - n];
                    array[index - n] = t;
                }
            }
            n /= 3;
        }
    }

    public static void main(String[] args) {
        Random rand = new Random();
        int[] array = new int[100000];

        for (int i = 0; i < array.length; i++) {
            array[i] = rand.nextInt();
        }
        long start = System.currentTimeMillis();
        shellSort(array);
        System.out.println(System.currentTimeMillis() - start);

//        for (int i = 0; i < array.length; i++) {
//            array[i] = rand.nextInt();
//        }
//        start = System.currentTimeMillis();
//        Arrays.sort(array);
//        System.out.println(System.currentTimeMillis() - start);
    }
}
