//ファイル入出力
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
int main() {
	char fname[256], word[256], tmp[256];
	int count = 0;
	cout << "ファイル名を入力してください";
	cin >> fname;
	ifstream fin(fname);
	if (!fin) {
		cout << "入力ファイルが開けません";
		return 1;
	}
	cout<<"検索語入力：";
	cin >> word;
	//ファイルを最後まで検索, EOF=End of File
	//eof()がtrueになるまで繰り返す
	while (!fin.eof()) {
		fin >> tmp; //一旦、tmpに落とす
		if (strcmp(word, tmp)==0) {
			count++;
		}
	}
	fin.close();
	cout << "単語数=" << count << endl;//endl=End Line(\n)
	return 0;
}