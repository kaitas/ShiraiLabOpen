//�t�@�C�����o��
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
int main() {
	char fname[256], word[256], tmp[256];
	int count = 0;
	cout << "�t�@�C��������͂��Ă�������";
	cin >> fname;
	ifstream fin(fname);
	if (!fin) {
		cout << "���̓t�@�C�����J���܂���";
		return 1;
	}
	cout<<"��������́F";
	cin >> word;
	//�t�@�C�����Ō�܂Ō���, EOF=End of File
	//eof()��true�ɂȂ�܂ŌJ��Ԃ�
	while (!fin.eof()) {
		fin >> tmp; //��U�Atmp�ɗ��Ƃ�
		if (strcmp(word, tmp)==0) {
			count++;
		}
	}
	fin.close();
	cout << "�P�ꐔ=" << count << endl;//endl=End Line(\n)
	return 0;
}