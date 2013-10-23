import java.util.Scanner;

// メソッドの基本理解 p.222～
// 一つの整数型の値 a をキーボードから読み込み
// a の階乗を求めるプログラムを作成しなさい。
// そのとき、階乗計算は factrial という名前でメソッドとして作成すること。
// ヒント：教科書のp.230にべき乗を求めるプログラム例がある


public class Sample0404 {

	//xのn乗をかえすメソッド
	static double power(double x, int n) {
		double tmp = 1.0f;
		for (int i=1; i<=n ; i++) {
			tmp *= x;
		}
		return tmp;
	}
	// aの階乗を返すメソッド
	static int factorial (int a) {
		int sum = 1;
		for (int i=0; i<a; i++) {
			sum *= (i+1);
		}
		return sum;
	}

	public static void main(String[] args) {
		Scanner stdin = new Scanner(System.in);
		int a;
		System.out.print("数値a="); a=stdin.nextInt();
		System.out.println("階乗:"+factorial(a));


	}

}
