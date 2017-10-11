import java.util.Random;

public class SelectSort {

    private static void selectSort(int[] array) {
        int current = 0, min = 0;
        int t = 0;
        for (current = 0; current < array.length - 1; current++) {
            min = current;
            for (int i = current + 1; i < array.length; i++) {
                if (array[i] < array[min]) {
                    min = i;
                }
            }
            t = array[min];
            array[min] = array[current];
            array[current] = t;
        }
    }

    public static void main(String[] args) {
        int[] array = new int[100000];
        Random rand = new Random();
        for (int i = 0; i < array.length; i++) {
            array[i] = rand.nextInt();
        }

        long start = System.currentTimeMillis();
        selectSort(array);
        System.out.println(System.currentTimeMillis() - start);
    }
}
