/* sample09021.java
 * 円錐の体積を求める
 * importを使わない記述（p.342）
 * 円錐＝（底面の半径×底面の半径×円周率×高さ）÷３
 * 円柱＝  底面の半径×底面の半径×円周率×高さ
 * */
public class sample09021 {

	public static void main(String[] args) {
		java.util.Scanner stdIn = new java.util.Scanner(System.in);
		System.out.println("底面の半径:");
		double r = stdIn.nextDouble();
		System.out.println("高さ:");
		double h = stdIn.nextDouble();

		System.out.println("円錐の体積"+ (r * r * h * Math.PI  / 3.0f) );
	}

}
