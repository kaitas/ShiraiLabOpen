/*
 * 【必須問題 0602】 出題意図：
 * 日付クラスインスタンスの比較 演習プログラム 0602  ファイル名:  sample0601.java
 * 【0602】のソースコード中 Main メソッドに Day クラスのインスタンス xday を作成し、
 * プログラム中で自分 の誕生日を与えて初期化する。次に【0601】で作成したインスタンス birthday
 * との比較を if (birthday == xday) 文を用いて行い、等しい場合に「等しいです」
 * 等しくない場合に「等しくありません」のメッセージを出力する 文を追加し、
 * 実行結果を確かめよ。ファイルは sample0601.java に上書きして作成して良い。
 */

import java.util.Scanner;
//教科書のp.302 List 9-1より
class Day2 {
	private int year;
	private int month;
	private int date;
	//Constructor
	Day (int year, int month, int date) {
		this.year  = year;
		this.month = month;
		this.date  = date;
	}
	//get
	int getYear()  {return year;}
	int getMonth() {return month;}
	int getDate()  {return date;}
	//set
	void setYear(int year)  {this.year = year;}
	void setMonth(int month) {this.month = month;}
	void setDate(int date)  {this.date = date;}
	//set(date specified)
	void set(int year, int month, int date) {
		this.year = year;
		this.month = month;
		this.date = date;
	}
	//obtain day of week 曜日を求める関数
	int dayOfWeek() {
		int y = year;
		int m = month;
        if (m == 1 || m== 2) {
        	y--;
        	m+=12;
        }
	 return (y + y/4 - y/100 + y/400 + (13*m+8)/5 + date) % 7;
	}
}  // end of class

//教科書のp.303 List 9-2より
public class Sample0602 {
	public static void main(String[] args) {
		Scanner stdin = new Scanner(System.in);
		String[] wd = {"日","月","火","水","木","金","土"};
		System.out.println("誕生日を入力してください");
		System.out.println("年:"); int y = stdin.nextInt();
		System.out.println("月:"); int m = stdin.nextInt();
		System.out.println("日:"); int d = stdin.nextInt();
		Day birthday = new Day(y,m,d);
		Day xday = new Day(1973,3,13);
		if (birthday==xday) {
			System.out.println("等しいです");
		} else {
			System.out.println("等しくないです");
		}
	}

}
