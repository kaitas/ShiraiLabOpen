/*【必須問題 0601】 出題意図： 日付クラスの入力と実行
 *  演習プログラム 0601  ファイル名:  sample0601.java
 *  教科書 302 ページの日付クラス Day【第１版】および
 *  303 ページにある日付クラス Day【第１版】利用例の
 *  クラス名を sample0601 に変更して入力し、
 *  自分の誕生日の曜日を出力せよ。
 */

import java.util.Scanner;
//教科書のp.302 List 9-1より
class Day1 {
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
public class sample0601 {
	public static void main(String[] args) {
		Scanner stdin = new Scanner(System.in);
		String[] wd = {"日","月","火","水","木","金","土"};
		System.out.println("誕生日を入力してください");
		System.out.println("年:"); int y = stdin.nextInt();
		System.out.println("月:"); int m = stdin.nextInt();
		System.out.println("日:"); int d = stdin.nextInt();
		Day birthday = new Day(y,m,d);
		System.out.println("誕生日は"
			+birthday.getYear() +"年"
			+birthday.getMonth()+"月"
			+birthday.getDate() +"日"
			+wd[birthday.dayOfWeek()]+"曜日です");
	}

}
