// 書式設定された入出力
#include <iostream>
using namespace std;
int main () {
	int i;
	double x;
	char str[100];

	cout << "整数を入力してください";
	cin >> i;
	cout << "実数を入力してください";
	cin >> x;
	cout << "文字列を入力してください";
	cin >> str;

	cout << "デフォルト表示\n";
	cout << i << "\n";
	cout << x << "\n";
	cout << str << "\n";
	//formatting1
	cout.unsetf(ios::dec);
	cout.setf(ios::hex|ios::showbase|
		ios::showpos );
	cout << "\n16進数, 基数表示, 正符号表示\n";
	cout << i << "\n";
	cout << x << "\n";
	cout << str << "\n";

	//formatting2
	cout.unsetf(ios::hex|ios::showbase|
		ios::showpos);
	cout.setf(ios::dec|ios::scientific|
		ios::showpoint );
	cout << "\n10進数, 科学技術表示, 小数点と後続ゼロ表示\n";
	cout << i << "\n";
	cout << x << "\n";
	cout << str << "\n";

	return 0;
}