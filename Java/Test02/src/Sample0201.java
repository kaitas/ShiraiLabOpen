import java.util.Scanner;

//演習プログラム0201
//2013/10/2 1234567 白井暁彦


public class Sample0201 {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Scanner stdIn = new Scanner(System.in);
		System.out.println("2つの入力された数値を掛け算します");
		System.out.print("数値1");
		int num1 = stdIn.nextInt();
		System.out.print("数値2");
		int num2 = stdIn.nextInt();
		System.out.println(num1+" x "+num2+" = "+(num1*num2));

	}

}
