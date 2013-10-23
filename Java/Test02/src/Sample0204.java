//課題204: ifによる分岐
// 1:整数値をひとつ読み込む
// 2:もし、0以下であれば、エラー
// 3:そうでなく、さらに12より大きければこれもエラー
// 4:それ以外のものは「○月を意味します」と表示
import java.util.Scanner;

public class Sample0204 {
	public static void main(String[] args) {
		Scanner stdIn = new Scanner(System.in);
		System.out.print("整数値をひとつおねがい：");
		int m = stdIn.nextInt();
		if (m <=0 ) {
			System.out.println(m+"月のような月はありません");
		} else {
			if ( m > 12 ) {
				System.out.println(m+"月のような月はありません");
			} else {
				System.out.println("入力値は"+m+"月を意味します");
			}
		}
	}
}
