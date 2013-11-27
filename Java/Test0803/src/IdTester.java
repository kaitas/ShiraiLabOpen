/* 0803
 * IdTester.java
 * 教科書p.351ページ　演習10-1
 * List10-3(p.340)に示した連番クラスIdに、
 * 最後に与えた識別番号を返すクラスメソッド
 * getMaxIdを追加せよ
 *  static int getMaxId()
 * なお、このメソッドは、インスタンスをn個作成した時点で呼び出すと
 * nを返すことになる
 * */

class Id {
	static int counter = 0;
	private int id;
	public Id() {
		id = ++counter;
	}
	public int getId() {
		return id;
	}
	//追加
	static int getMaxId() {
		return counter;
	}
}

public class IdTester {
	public static void main(String[] args) {
		Id a = new Id();
		Id b = new Id();
		System.out.println("aの識別番号："+a.getId());
		System.out.println("bの識別番号："+b.getId());

		System.out.println("Id.counter ="+Id.counter);
		System.out.println(" a.counter ="+ a.counter);
		System.out.println(" b.counter ="+ b.counter);

		//コンストラクタを呼びまくってみる
		Id c = new Id();
		Id d = new Id();
		Id e = new Id();
		Id f = new Id();
		System.out.println(" f.getMaxId() ="+ f.getMaxId());
		System.out.println(" a.getMaxId() ="+ a.getMaxId());

	}

}
