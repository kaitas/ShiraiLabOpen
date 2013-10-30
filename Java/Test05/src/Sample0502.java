/* Sample0502 メソッドの多重定義
 * 2個の整数の平均値および3個の数値の平均を求めるaverageというメソッドを、
 * 入力した2個と3個の数値がそれぞれすべて、int/float型の場合でメソッドを多重定義しなさい。
 */
public class Sample0502 {
	static double average(int a, int b) {
		double ave;
		ave = (double)(a+b);
		ave/=2;
		return ave;
	}
	static double average (int a, int b, int c) {
		double ave;
		ave = (double)(a+b+c);
		ave /=3;
		return ave;
	}
	static double average(double a, double b) {
		double ave;
		ave = (a+b)/2.0f;
		return ave;
	}
	static double average(double a, double b, double c) {
		double ave;
		ave = (a+b+c)/3.0f;
		return ave;
	}
	public static void main(String[] args) {
		//テストデータ
		int a = 1, b=2, c=3; double d=4, e=5, f=6;
		System.out.println("average( "+a+", "+b+" ) = "+average(a,b));
		System.out.println("average( "+a+", "+b+" , "+ c +" ) = "+average(a,b,c));
		System.out.println("average( "+d+", "+e+" ) = "+average(d,e));
		System.out.println("average( "+d+", "+e+" , "+ f +" ) = "+average(d,e,f));


	}

}
