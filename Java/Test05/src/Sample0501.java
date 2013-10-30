/* Sample 0501
 * 学籍番号1番から5番の学生の「応用プログラミングB」の成績（0～100点）を
 * キーボードから読み込み、最高点と最高点だった学生の学籍番号を表示するプログラムを作成しなさい
*/
import java.util.Scanner;

public class Sample0501 {
	public static void main(String[] args) {
		Scanner stdin = new Scanner (System.in);
		int [] seiseki = new int[5];
		int max = 0;  // 最高点の学生のID
		for (int i=0; i<seiseki.length; i++) {
			System.out.println("学籍番号"+(i+1)+":");
			seiseki[i] = stdin.nextInt();
			if (seiseki[max]<seiseki[i]) {
				max = i;
			}
		}
		System.out.println("最高点をとった人="+(max+1)+" 点数="+seiseki[max]);
	}

}
