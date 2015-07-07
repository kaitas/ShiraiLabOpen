#include <iostream>
using namespace std;
//ここに関数を3つオーバーロードさせて書く(p.29参照)
char max(char a, char b);
int max(int a, int b);
double max(double a, double b);
char max(char a, char b) {
	return a>b ? a: b;
}
int max(int a, int b) {
	return a>b ? a: b;
}
double max(double a, double b) {
	return a>b ? a: b;
}
int main() {
	//テスト用のデータ
	char c1 = 'a', c2 = 'b';
	int i1 = 10, i2 = 20;
	double d1 = 1.01, d2 = 0.99;

	cout << c1 << "と" << c2 << "を比較して大きいのは" 
		<< max(c1, c2) << "です\n";

	cout << i1 << "と" << i2 << "を比較して大きいのは" 
		<< max(i1, i2) << "です\n";

	cout << d1 << "と" << d2 << "を比較して大きいのは" 
		<< max(d1, d2) << "です\n";

	return 0;
}
