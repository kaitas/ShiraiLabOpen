//�o�C�i���t�@�C��(BMP)���o��
#include <iostream>
#include <fstream>
using namespace std;
int main() {
	char FileNameIN[256];
	char FileNameOUT[256];
	char header[54]; // for BMP file header
	char b;
	cout << "���̓t�@�C�������w�肵�Ă��������F";
	cin >> FileNameIN;
	//�o�C�i���w��Ńt�@�C������͗p�ŊJ��
	ifstream BMPFile1(FileNameIN,ios::in | ios::binary );
	if (!BMPFile1) {
		cout << "���̓t�@�C�� " << FileNameIN << " ���J���܂���";
		return 1;
	}
	cout << "�o�̓t�@�C�������w�肵�Ă��������F";
	cin >> FileNameOUT;
	//�o�C�i���w��Ńt�@�C�����o�͗p�ɊJ��
	ofstream BMPFile2(FileNameOUT, ios::out | ios::binary );
	if (!BMPFile2) {
		cout << "�o�̓t�@�C�����J���܂���\n";
		return 1;
	}
	BMPFile1.read(header,54); //�w�b�_54�o�C�g��ǂݍ���
	BMPFile2.write(header,54); //���������o��file�ɂ������������
	while (!BMPFile1.eof()) {
		BMPFile1.get(b); //��f1����char�^�ŃR�s�[��
		BMPFile2.put(~b); //�`���_���g�����ƂŔ��]������(��)0��255
	}
	BMPFile1.close();	BMPFile2.close();
	return 0;
}