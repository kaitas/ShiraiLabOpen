//ファイル出力の基本
#include <iostream>
#include <fstream>
using namespace std;

int main () {
	char str[256];
	ofstream fout("test.txt");
	if (!fout) {
		cout << "出力ファイルが開けません\n";
	}
	cout << "文字列を入力してください：";
	cin >> str;
	fout << str; //指定した文字列がファイルに書き込まれる
	fout.close(); //ファイルを閉じるのを忘れずに
	return 0;
}
