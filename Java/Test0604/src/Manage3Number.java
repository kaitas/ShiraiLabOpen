/**
 * 3 つの整数値を管理するクラス Manage3Number を定義する。
 * このクラスは、3 つの値を設定する(set)、合計 する(sum)、
 * 合計を表示する(printSum)のメソッドを持つものとする。
 * コンストラクタは、与えられる引数の数 が 0,1,2,3 個の場合について定義しておく。
 * メンバー変数は num1, num2, num3 とし、値の初期化をする場合には、
 * 必ず num1 から num2, num3 の順に設定され、与えられなかった数値は 0 とする。
 */

public class Manage3Number {
	int num1,num2,num3;
	//Constructors
	Manage3Number() {setNum(0,0,0);}
	Manage3Number(int n1) {setNum(n1,0,0);}
	Manage3Number(int n1,int n2) {setNum(n1,n2,0);}
	Manage3Number(int n1, int n2, int n3) {setNum(n1,n2,n3);}
	//Setters
	void setNum(int n1, int n2, int n3) {
		num1 = n1;
		num2 = n2;
		num3 = n3;
	}
	void setNum1(int n) {num1 = n;}
	void setNum2(int n) {num2 = n;}
	void setNum3(int n) {num3 = n;}


	int sum() {
		return (num1+num2+num3);
	}
	void printSum() {
		System.out.println("合計:"+sum());
	}

}
