/* 第13回 教科書15章の演習
 * sample1301,sample1302
 * 文字と文字列の厳密な理解
 * */

import java.util.Scanner;

public class sample1301 {

	public static void main(String[] args) {
		System.out.println("\n--課題1--\n");
		//繰り返し１：文字リテラルのカウントアップ
		char moji = '0';
		System.out.println(moji);
		for (int i=0 ; i<9 ; i++ ){
			moji = (char) (moji +1);
    		System.out.println(moji);
		}
		System.out.println("\n----\n");
		//繰り返し２：文字列、for文でcharをつかってみた
		String mojiretsu = new String();
		for (char c = 'A'; c <='Z'; c = (char)(c+1) ) {
			mojiretsu = mojiretsu + c;
    		System.out.println(mojiretsu);
		}
		System.out.println("\n----\n");
		System.out.println(mojiretsu);

		System.out.println("\n--課題2の実験--\n");
		Scanner stdIn = new Scanner(System.in);
		String s2 = new String();
		s2 = "ABCDEFG";
		System.out.println(s2);
		System.out.println(" 検索対象文字列:");
		String target = stdIn.next();
		System.out.println(" 置換対象文字列:");
		String rep = stdIn.next();
		System.out.println(s2.replace(target, rep));

		System.out.println("\n--課題2--\n文字列 T R を入力してください：");
		String k2 = stdIn.next();
		String Strs[] = k2.split(" "); //スペース区切りでString配列にバラす
		System.out.println(" 対象文字列:"+Strs[0]);
		System.out.println(" 検索文字列:"+Strs[1]);
		System.out.println(" 置換文字列:"+Strs[2]);

		System.out.println("\n--課題3--\n");
		System.out.println("元の文字列:"+s2);
		System.out.print("逆転後の文字列:");
		for (int i=1; i<=s2.length() ; i++) {
			System.out.print(s2.charAt(s2.length()-i));
		}

	}

}
