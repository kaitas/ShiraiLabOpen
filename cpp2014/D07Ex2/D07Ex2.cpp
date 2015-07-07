//値渡し、ポインタ渡し、参照渡し
#include <iostream>
using namespace std;

class Coord {
	double x, y, z;
public:
	void set(double u, double v, double w) {
		x = u;
		y = v;
		z = w;
	}
	void show();
	void setCoord(Coord ob);
};

void Coord::show() {
	cout << "(" << x << "," << y << "," << z << ")";
}

//値渡し(クラス内で実験してみる)
void Coord::setCoord(Coord ob) {
	double u,v,w;
	cout << "数字を3つ入力してください:";
	cin >> u >> v >> w;
//	ob.set(u,v,w);
	set(u,v,w);
}


//値渡し
void setCoord1 (Coord ob) {
	double u,v,w;
	cout << "数字を3つ入力してください:";
	cin >> u >> v >> w;
	ob.set(u,v,w);
}

//ポインタ渡し
void setCoord2 (Coord *ob) { //&で渡したポインタは*で受け取る!!これを"ポインタ渡し"と呼ぶ
	double u,v,w;
	cout << "数字を3つ入力してください:";
	cin >> u >> v >> w;
	ob->set(u,v,w); //ポインタでもらったらアロー演算子で実体にアクセス
}

//参照渡し
void setCoord3 (Coord &ob) { //無印で渡して&で受け取る!!これを"参照渡し"と呼ぶ
	double u,v,w;
	cout << "数字を3つ入力してください:";
	cin >> u >> v >> w;
	ob.set(u,v,w); //参照仮引数
}


int main() {
	Coord obj;
	obj.set(1.1, 2.3, 5.5);
	cout << "オブジェクトの初期値:";
	obj.show();
	cout << "\n\n";
	cout << "値渡しで関数にオブジェクトを渡します\n";
	setCoord1(obj);
//	obj.setCoord(obj);
	cout << "オブジェクトの値：";
	obj.show(); //実際にはここで値は変わらない
	cout << "\n\n";

	cout << "ポインタ渡しで関数にオブジェクトを渡します\n";
	setCoord2(&obj);
	cout << "オブジェクトの値：";
	obj.show(); //値は変わる？→Yes
	cout << "\n\n";

	cout << "参照渡しで関数にオブジェクトを渡します\n";
	setCoord3(obj);
	cout << "オブジェクトの値：";
	obj.show(); //値は変わる？
	cout << "\n\n";

	return 0;
}