//バイナリファイル(BMP)入出力
#include <iostream>
#include <fstream>
using namespace std;
int main() {
	char FileNameIN[256];
	char FileNameOUT[256];
	char header[54]; // for BMP file header
	char b;
	cout << "入力ファイル名を指定してください：";
	cin >> FileNameIN;
	//バイナリ指定でファイルを入力用で開く
	ifstream BMPFile1(FileNameIN,ios::in | ios::binary );
	if (!BMPFile1) {
		cout << "入力ファイル " << FileNameIN << " が開けません";
		return 1;
	}
	cout << "出力ファイル名を指定してください：";
	cin >> FileNameOUT;
	//バイナリ指定でファイルを出力用に開く
	ofstream BMPFile2(FileNameOUT, ios::out | ios::binary );
	if (!BMPFile2) {
		cout << "出力ファイルが開けません\n";
		return 1;
	}
	BMPFile1.read(header,54); //ヘッダ54バイトを読み込む
	BMPFile2.write(header,54); //すかさず出力fileにそれを書き込む
	while (!BMPFile1.eof()) {
		BMPFile1.get(b); //画素1つずつはchar型でコピー可
		BMPFile2.put(~b); //チルダを使うことで反転させる(例)0→255
	}
	BMPFile1.close();	BMPFile2.close();
	return 0;
}