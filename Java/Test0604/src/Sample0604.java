/*
 * Sample0604
 * */
public class Sample0604 {

	/**
3 つの整数値を管理するクラス Manage3Number を定義する。
このクラスは、3 つの値を設定する(set)、合計 する(sum)、
合計を表示する(printSum)のメソッドを持つものとする。
コンストラクタは、与えられる引数の数 が 0,1,2,3 個の場合について定義しておく。
メンバー変数は num1, num2, num3 とし、値の初期化をする場合に
は、必ず num1 から num2, num3 の順に設定され、与えられなかった数値は 0 とする。
上記クラスを定義し、 Main メソッドで Manage3Number の配列型インスタンス data[4]を宣言し、
data[0]は引数 0 のコンストラクタ で、
data[1]は 1 つの初期値を与えるコンストラクタで、
data[2]は 2 つの初期値を、
data[3]は 3 つの初期値を引 数とするコンストラクタで初期化し、
それぞれ合計値を求めるプログラムを作成しなさい。
また data[0]に値を 3 つ与えてセットしなおし合計値を表示する文を追加しなさい。
クラスごとにソースファイルを分割し、 Manage3Number.java と sample0604.java を作成すること。
	 */
	public static void main(String[] args) {
		Manage3Number [] data = new Manage3Number[4];
		data[0] = new Manage3Number();
		data[1] = new Manage3Number(1);
		data[2] = new Manage3Number(1,2);
		data[3] = new Manage3Number(1,2,3);
		for (int i = 0 ; i<data.length; i++) {
			data[i].printSum();
		}
		data[0].setNum(4, 5, 6);
		data[0].printSum();
	}

}
