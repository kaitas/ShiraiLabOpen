/* 必須問題0901：パッケージ作成方法の理解
 * 教科書p.376 List 11-5より
*/

package id;

import java.util.Random;


public class RandId {
	private static int counter;
	private int id;
	static {
		Random rand = new Random();
		counter = rand.nextInt();
	}

	//コンストラクタ
	public RandId() {
		id = ++counter;
	}

	//識別番号を取得
	public int getId() {
		return id;
	}

}
