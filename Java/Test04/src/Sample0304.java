// 演習プログラム 0304
// 2013/10/9 白井暁彦
// 教科書p.144参照
import java.util.Scanner;

public class Sample0304 {

	public static void main(String[] args) {
		Scanner stdIn = new Scanner(System.in);
		//byte型の上限確認
		byte b = (byte)127;
		System.out.println("(byte)"+ b+":"+(byte)(b+1)+":"+(byte)(b+2));

		//short型の上限確認
		short s = (short)32767;
		System.out.println("(short)"+ s+":"+(short)(s+1)+":"+(short)(s+2));

		//long型の上限確認
		long l = (long)(9223372036854775807L);
		System.out.println("(long)"+ l+":"+(long)(l+1)+":"+(long)(l+2));

	}

}
