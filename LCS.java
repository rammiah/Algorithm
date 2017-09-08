public class LCS {
    private static int lcs(String str1, String str2) {
        if (str1 == null || str2 == null ||
                str1.length() == 0 || str2.length() == 0) return 0;
        char[] chs1 = str1.toCharArray(), chs2 = str2.toCharArray();

        int max = 0;
        int current = 0;
        int[][] temp = new int[chs1.length][chs2.length];
        for (int i = 0; i < chs1.length; i++) {
            temp[i][0] = chs1[i] == chs2[0] ? 1 : 0;
        }
        for (int i = 1; i < chs2.length; i++) {
            temp[0][i] = chs2[i] == chs1[0] ? 1 : 0;
        }
        for (int i = 1; i < chs1.length; i++) {
            for (int j = 1; j < chs2.length; j++) {
                temp[i][j] = chs1[i] == chs2[j] ? temp[i - 1][j - 1] + 1 : 0;
                max = temp[i][j] > max ? temp[i][j] : max;
            }
        }

        return max;
    }

    public static void main(String[] args) {
        System.out.println(lcs("123456", "134"));
    }
}

