//�t�@�C���o�͂̊�{
#include <iostream>
#include <fstream>
using namespace std;

int main () {
	char str[256];
	ofstream fout("test.txt");
	if (!fout) {
		cout << "�o�̓t�@�C�����J���܂���\n";
	}
	cout << "���������͂��Ă��������F";
	cin >> str;
	fout << str; //�w�肵�������񂪃t�@�C���ɏ������܂��
	fout.close(); //�t�@�C�������̂�Y�ꂸ��
	return 0;
}
