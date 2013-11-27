/*
 * Sample0801
 * 教科書p.350のリスト7-3がベースになっています
 * */
import java.util.Scanner;

public class Sample0801 {
	// --- a, b. c の最大値を返却---//
	static int max(int a, int b, int c) {
		int max = a;
		if (b > max) max = b;
		if (c > max) max = c;
		return max;
	}
	/*staticつきで宣言されているmainメソッドは、
	 * クラスメソッド（静的メソッド）です。
	 * クラスメソッドからは、同一クラスのインスタンスメソッドは呼べませんので、
	 * maxにもstaticをつけてクラスメソッドとしなければなりません。
	 * もしstaticをつけずに非静的メソッドとして宣言されると、
	 * mainメソッドから呼び出せなくなります（コンパイルエラー）*/
	// --- a, b. c の最小値を返却---//
	 static int min(int a, int b, int c) {
		int min = a;
		if (b < min) min = b;
		if (c < min) min = c;
		return min;
	}

	public static void main(String[] args) {
		Scanner stdIn = new Scanner (System.in);
		System.out.println("整数a:"); int a = stdIn.nextInt();
		System.out.println("整数b:"); int b = stdIn.nextInt();
		System.out.println("整数c:"); int c = stdIn.nextInt();
		System.out.println("最小値は"+min(a,b,c)+"です。");
		System.out.println("最大値は"+max(a,b,c)+"です。");
	}

}
