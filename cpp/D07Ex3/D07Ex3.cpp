//ポインタの復習
#include <iostream>
using namespace std;

class samp {
	int a, b;
public: 
	samp (int n, int m) {
		a = n;
		b = m;
	}
	void show() {
		cout << "(" << a << "," << b << ")\n"; 
	}
};

int main () {
	//実行結果を参考に配列オブジェクトを初期化
	samp ob[3][2] = {
		samp(1,
		2), samp(2,3),
		samp(3,4), samp(4,5),
		samp(5,6), samp(6,7)
	};
	samp *p;
	p = (samp *) ob; //このポインタ渡しでは配列が渡るわけではない、配列の先頭のみがpに入る
	for (int i = 0; i <6; i++) {
		//ポインタを使って出力
		p->show();
		//ポインタの示すアドレスを1つ後ろにする
		p++;
	}
	return 0;
}