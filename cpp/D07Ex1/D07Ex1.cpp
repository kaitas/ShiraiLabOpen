#include <iostream>
using namespace std;
// ポインタ仮引数→参照仮引数を使って書き換え

//void f(int *n) {
void f(int &n) {
	n = n * 2;
}

int main() {
	int i = 10;
	cout << " i = " << i << "\n";
//	f(&i);
	f(i);
	cout << " i = " << i << "\n";

	return 0;
}