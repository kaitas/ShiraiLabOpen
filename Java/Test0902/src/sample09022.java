/* sample09022.java
 * 円錐の体積を求める
 * Math.PIにもimportを使う記述（p.370）
 * 円錐＝（底面の半径×底面の半径×円周率×高さ）÷３
 * 円柱＝底面の半径×底面の半径×円周率×高さ
 * */
import static java.lang.Math.*;

import java.util.Scanner;

public class sample09022 {
	public static void main(String[] args) {
		Scanner stdIn = new Scanner(System.in);
		System.out.println("底面の半径:");
		double r = stdIn.nextDouble();
		System.out.println("高さ:");
		double h = stdIn.nextDouble();

		System.out.println("円錐の体積"+ (r * r * h * PI  / 3.0) );
	}

}

