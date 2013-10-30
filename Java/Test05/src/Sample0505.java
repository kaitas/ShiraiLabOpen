/* Sample0505 クラスの基本概念理解
 * 名前文字列と学籍番号（1000-2000の整数）と1科目の試験の点数(0-100)を1つのクラス
 * basicSeisekiBetaとして定義し、3人分の学生データをインスタンス化し、
 * データをプログラム内で固定値として格納し、すべての値を表示する処理を実現する(p.270)
 * */

class basicSeisekiBeta {
	String name; int num; int point;
}

public class Sample0505 {
	public static void main(String[] args) {
		basicSeisekiBeta data1 = new basicSeisekiBeta();
		basicSeisekiBeta data2 = new basicSeisekiBeta();
		basicSeisekiBeta data3 = new basicSeisekiBeta();
		data1.name = "Akihiko SHIRAI"; data1.num = 1001; data1.point = 50;
		data2.name = "Akira ISHIKAWA"; data2.num = 1002; data2.point = 80;
		data3.name = "Masaki GOUDO";   data3.num = 1003; data3.point = 90;

		System.out.println("名前:"+data1.name+" 学籍番号:"+data1.num+" 点数"+data1.point);
		System.out.println("名前:"+data2.name+" 学籍番号:"+data2.num+" 点数"+data2.point);
		System.out.println("名前:"+data3.name+" 学籍番号:"+data3.num+" 点数"+data3.point);
	}
}
