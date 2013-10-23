//課題205: ifによる分岐2
//ifをひとつで実装する

import java.util.Scanner;
public class Sample0205 {
	public static void main(String[] args) {
		Scanner stdIn = new Scanner(System.in);
		System.out.print("整数値をひとつおねがい：");
		int m = stdIn.nextInt();
		//mが0以下、もしくは12よりおおきければエラー
		if ( (m <=0 ) || ( m > 12 ) ) {
			System.out.println(m+"月のような月はありません");
		} else {
			System.out.println("入力値は"+m+"月を意味します");
		}
	}
}
