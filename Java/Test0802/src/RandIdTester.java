/*
 * Sample0802
 * P.354 List 10-11
 * */
import java.util.Random;

class RandId {
	private static int counter;
	private int id;
	//静的初期化子
	static {
		Random rand = new Random();
//		counter = rand.nextInt(10)* 100;
		counter = rand.nextInt(10)* 1000;
		System.out.println("here");
	}
	/*クラス初期化子(class initializer)もしくは静的初期化子(static initializer)と呼ばれる。
	 * もしくはstaticブロックと呼ばれる
	 * クラスが初期化される際に実行される
	 * クラスが初期化されるタイミングは、
	 * ・そのクラスのインスタンスが生成される
	 * ・そのクラスのクラスメソッドが呼び出される
	 * ・そのクラスのクラス変数に値が代入される
	 * ・そのクラスの定数ではないクラス変数の値が取り出される*/
	//コンストラクタ
	public RandId() {
		id = ++counter;
		id = ++counter;
	}
	public int getId() {
		return id;
	}
}

public class RandIdTester {
	public static void main(String[] args) {
		RandId a = new RandId();
		RandId b = new RandId();
		RandId c = new RandId();
		System.out.println("aの識別番号:"+a.getId());
		System.out.println("bの識別番号:"+b.getId());
		System.out.println("cの識別番号:"+c.getId());

	}
}
