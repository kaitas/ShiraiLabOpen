//課題2-6
//繰り返し記述3種類
import java.util.Scanner;
public class Sample0206 {
	public static void main(String[] args) {
		Scanner stdIn = new Scanner(System.in);
		System.out.println("30より大きな整数を1つください");
		int x = stdIn.nextInt();
		//2の倍数をdoで
		System.out.print("【2の倍数】");
		int i=1;
		do {
			if (i%2==0) {
				System.out.print(" ["+i+"]");
			}
			i++;
		}
		while (i<=x);
		System.out.println("");
		//3の倍数をwhileで
		i = 1;
		System.out.print("【3の倍数】");
		while (i<=x) {
			if (i%3==0) {
				System.out.print(" ["+i+"]");
			}
			i++;
		}
		System.out.println("");
		//5の倍数をforで
		System.out.print("【5の倍数】");
		for (i=1; i<=x; i++) {
			if (i%5==0) {
				System.out.print(" ["+i+"]");
			}
		}


	}

}
