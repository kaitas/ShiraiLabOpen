#include <iostream>
#include <cstring>
using namespace std;

class CName; //前方宣言(p.98)は必要
class Circle {
	double x,y;
	double radius;
	int r,g,b;
public:
	void set_xyr(double s, double t, double r) {
		x = s; y = t; radius = r;
	}
	void set_color(int u, int v, int w) {
		r = u; g = v; b = w;
	}
	void show();
	bool isSame(CName col); //CNameのメンバではないので注意
};

class CName {
	char name[50];
	int r,g,b;
public:
	CName (char* str, int u, int v, int w);
	char* get_name() {return name;}
	void show();
	friend bool Circle::isSame(CName col); 
	//フレンド関数として宣言して,メンバではないisSameにCNameの変数(r,g,b)に直接アクセスさせることを許可
};

void Circle::show() {
	cout << "座標：(" << x << "," << y << ")\n"; 
	cout << "半径：(" << radius << ")\n"; 
	cout << "RGB：(" << r << "," << g << "," << b << ")\n"; 
}

bool Circle::isSame(CName col) {
	col.r++;
	if (r==col.r && g==col.g && b==col.b) return true;
	else return false;
}

//CNameのコンストラクタ、渡した名前をコピーしてRGBを渡す
CName::CName(char* str, int u, int v, int w) {
	strcpy(name, str);
	r = u; g = v; b = w;
}
void CName::show() {
	cout << name << " : (" << r << "," << g << "." << b << ")\n";
}

int main() {
	Circle c;
	//色名と色の値を定義してオブジェクト1-3として作成
	CName co1("Red", 255,0,0);
	CName co2("Green", 0,255,0);
	CName co3("Blue", 0,0,255);

	c.set_xyr(2,3,5);
	c.set_color(255,0,0); //赤い円になるはず

	cout << "円の情報を表示\n";
	c.show();

	cout << "\n";
	cout << "色(co1)の情報：";	co1.show();
	cout << "色(co2)の情報：";	co2.show();
	cout << "色(co3)の情報：";	co3.show();
	cout << "\n";

	cout << "この円(c)の色名は" << co1.get_name();
	if (c.isSame(co1)) 
		cout << "です\n";
	else 
		cout << "ではないようです\n";

	cout << "この円(c)の色名は" << co2.get_name();
	if (c.isSame(co2)) 
		cout << "です\n";
	else 
		cout << "ではないようです\n";

	cout << "この円(c)の色名は" << co3.get_name();
	if (c.isSame(co3)) 
		cout << "です\n";
	else 
		cout << "ではないようです\n";

	cout << "\nisSameでプライベート変数に手を加えました\n";
	cout << "色(co1)の情報：";	co1.show();
	cout << "色(co2)の情報：";	co2.show();
	cout << "色(co3)の情報：";	co3.show();
	cout << "\n";


	return 0;

}

