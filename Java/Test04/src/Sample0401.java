import java.util.Scanner;

public class Sample0401 {
	public static void main(String[] args) {
		Scanner stdin = new Scanner(System.in);
		int[] num = new int[10]; //int型の配列、名前はnum要素は10個
		double sum = 0, mean = 0;
		for (int i=0; i<num.length; i++)
		{
			System.out.println("数値["+(i+1)+"]=");
			num[i] = stdin.nextInt();
			sum += num[i]; //合計として足していく
		}
		mean = sum / (double)num.length; //平均を実数で求める
	System.out.println("平均値="+mean);
	}

}
