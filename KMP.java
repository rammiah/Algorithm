package _KMP;

import java.util.Scanner;

public class KMP {
	
	public static int[] preProcess(final String s) {  
	    int size = s.length();  
	    int[] result = new int[size];  
	    result[0] = 0; 
	    int j = 0;  
	    //循环计算  
	    for(int i=1;i<size;i++){  
	        while(j>0 && s.charAt(j) != s.charAt(i)){  
	            j = result[j];  
	        }  
	        if(s.charAt(j) == s.charAt(i)){  
	            j++; 
	        }  
	        //找到一个结果  
	        result[i] = j;  
	    }  
	   return result;  
	} 
	
	private static int Match(String target, String pattern) {
		int[] table = preProcess(pattern);
		int j = 0;
		int i = 0;
		while (i < target.length()) {
			j = 0;
			while(j < pattern.length() && i < target.length()){
				if(target.charAt(i + j) == pattern.charAt(j)){
					++j;
				}
				else {
					break;
				}
			}
			if (j == pattern.length()) {
				return i;
			}
			else if (j > 0){
				i += j - table[j];
			}
			else {
				i += 1;
			}
		}
		return -1;
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		String target = new String();
		String pattern = new String();
		Scanner in = new Scanner(System.in);
		System.out.println("Please enter the target string:");
		target = in.nextLine();
		System.out.println("Please enter the pattern string:");
		pattern = in.nextLine();
		System.out.println("The pattern is in " + Match(target, pattern) + " index of target string");
		in.close();
	}

}
