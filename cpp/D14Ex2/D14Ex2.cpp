//ファイル入力の基本
#include <iostream>
#include <fstream>
using namespace std;
int main() {
	char str[256], fname[256];
	cout << "ファイル名を入力してください";
	cin >> fname;
	ifstream fin(fname);
	if (!fin) {
		cout << "入力ファイルが開けません";
		return 1;
	}
	fin >> str;
	fin.close();
	cout << str;
	return 0;
}