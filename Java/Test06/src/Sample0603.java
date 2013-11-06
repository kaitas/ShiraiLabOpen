/*
 * 【必須問題 0603】 出題意図： 日付クラスインスタンスの比較 演習プログラム 0603
 * ファイル名:  sample0601.java
 * 教科書 308 ページを参考に 2 つの日付を比較するメソッド compDay を作成し、
 * 【0602】のプログラムに追加 しなさい。main メソッドで birthday と xday を比較して、
 * 等しい場合に「等しいです」等しくない場合に「等 しくありません」のメッセージを出力する文を
 * 追加して【0602】の結果と比較しなさい。ファイル名は sample0601.java のままで良い。
 *
 */

import java.util.Scanner;
//教科書のp.302 List 9-1より
class Day {
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
	//日付の一致を判定する関数
	static boolean compDay(Day d1, Day d2) {
		return  d1.getYear()  == d2.getYear() &&
				d1.getMonth() == d2.getMonth() &&
				d1.getDate()  == d2.getDate();
	}

}  // end of class

//教科書のp.303 List 9-2より
public class Sample0603 {
	public static void main(String[] args) {
		Scanner stdin = new Scanner(System.in);
		String[] wd = {"日","月","火","水","木","金","土"};
		System.out.println("誕生日を入力してください");
		System.out.println("年:"); int y = stdin.nextInt();
		System.out.println("月:"); int m = stdin.nextInt();
		System.out.println("日:"); int d = stdin.nextInt();
		Day birthday = new Day(y,m,d);
		Day xday = new Day(1973,3,13);
		if (xday.compDay(birthday,xday) ) {
			System.out.println("等しいです");
		} else {
			System.out.println("等しくないです");
		}
	}

}
