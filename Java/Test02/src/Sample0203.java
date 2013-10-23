import java.util.Random;
import java.util.Scanner;

//課題2-3:乱数の発生
//p.44から

public class Sample0203 {

	public static void main(String[] args) {

		Random rand = new Random();
		Scanner stdIn = new Scanner(System.in);

		System.out.print("整数値：");
		int x = stdIn.nextInt();

		for (int i=0; i<5; i++) {
			int lucky = rand.nextInt(10) - 5 ; //-5～+4の乱数
			System.out.print(" ["+(i+1)+"]"+(x+lucky));
		}


	}

}
