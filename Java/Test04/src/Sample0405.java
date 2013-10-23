import java.util.Scanner;

// メソッドをほかのメソッドから呼び出す
/* キーボードから一辺の長さ n を入力して n^2個の文字で正方形を
 * 描くプログラムを作成する。まず文字 c を n 個連続表示する
 * メソッド putChars (p.233)を作成し、さらに putChar メソッドを
 * 利用して、一辺 n の大きさの正方形を表示するメソッド putSquareを
 * 追加し、 mainメソッドの中で正方形の大きさ入力を受け付け、
 * 作成したメソッドを呼び出して正方形を表示する。
 * 文字 c はmain メソッドの中で定義してよい。
*/


public class Sample0405 {

	//値を返さないメソッドはvoid型
	static void putChar(char c, int n) {
		while (n-- >0) {
			System.out.print(c);
		}
	}
	static void putSquare(char c, int n) {
		for (int i=0; i<n; i++) {
			putChar(c,n);
			System.out.println();
		}
	}
	public static void main(String[] args) {
		Scanner stdin = new Scanner(System.in);
		int n; char c = '*'; // こういうのを花文字という
		System.out.println("長さ="); n = stdin.nextInt();
		putSquare(c,n);
	}

}
