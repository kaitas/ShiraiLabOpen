/* Sample0503 ビット演算
 * 2つのint型整数を読み込み、それらを2進数で表示し（教科書p.241,244）、さらに2つの整数の
 * ビットの並びを比較して、同じビットがある桁数をこたえるプログラムを作成しなさい。
 * */
import java.util.Scanner;
public class Sample0503 {
	static void printInBit(int x) {
		for (int i=31; i>=0; i--) {
			System.out.print( ( ( x >>> i & 1) == 1 ) ? '1':'0' );
		}
		System.out.println();
	}
	static int CountSameBits(int x, int y) {
		int counter = 0;
		for (int i=31; i>=0; i--) {
			if (( x >>>i&1) == (y>>>i&1))
				counter++;
		}
		return counter;
	}
	public static void main(String[] args) {
		Scanner stdin = new Scanner(System.in);
		System.out.print("数値1="); int a = stdin.nextInt();
		System.out.print("数値2="); int b = stdin.nextInt();
		System.out.print("数値1(bit)="); printInBit(a);
		System.out.print("数値2(bit)="); printInBit(b);
		System.out.print("同一ビット桁数=" + CountSameBits(a,b) ) ;
	}

}
