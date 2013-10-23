import java.util.Scanner;

//応用問題0406
/* 凹凸な多次元配列の理解
 * 3学年で1つのプロジェクトグループを作成した。
 * メンバーの構成は1年生7人、2年生5人、3年生4人である。
 * このメンバーのプログラミング能力を数値化して管理することにした。
 * 具体的には、学年ごとをそれぞれ1次元配列にし、その1次元配列を
 * 一つの配列にまとめて、全体を2次元配列で管理することとした。
 * 全員の能力値を0～100の整数で入力し、一つの2次元配列に格納した後、
 * その内容を表示するプログラムを作成しなさい。
 * */

public class Sample0406 {

	public static void main(String[] args) {
		Scanner stdin = new Scanner(System.in);
		int[] bac1 = new int[7];
		int[] bac2 = new int[5];
		int[] bac3 = new int[4];
		int[][] seiseki = new int[3][];
		System.out.println("1年生");
		for (int i=0; i<bac1.length; i++) {
			System.out.println((i+1)+"番");
			bac1[i] = stdin.nextInt();
		}
		System.out.println("2年生");
		for (int i=0; i<bac2.length; i++) {
			System.out.println((i+1)+"番");
			bac2[i] = stdin.nextInt();
		}
		System.out.println("3年生");
		for (int i=0; i<bac3.length; i++) {
			System.out.println((i+1)+"番");
			bac3[i] = stdin.nextInt();
		}
		seiseki[0] = bac1;
		seiseki[1] = bac2;
		seiseki[2] = bac3;
		for (int i=0; i<3; i++) {
			System.out.println((i+1)+"年生");
			for (int j=0; j<seiseki[i].length; j++) {
				System.out.printf(" %2d番 %3d",(j+1),seiseki[i][j]);
			}
			System.out.println();
		}

	}

}
