// �����ݒ肳�ꂽ���o��
#include <iostream>
using namespace std;
int main () {
	int i;
	double x;
	char str[100];

	cout << "��������͂��Ă�������";
	cin >> i;
	cout << "��������͂��Ă�������";
	cin >> x;
	cout << "���������͂��Ă�������";
	cin >> str;

	cout << "�f�t�H���g�\��\n";
	cout << i << "\n";
	cout << x << "\n";
	cout << str << "\n";
	//formatting1
	cout.unsetf(ios::dec);
	cout.setf(ios::hex|ios::showbase|
		ios::showpos );
	cout << "\n16�i��, ��\��, �������\��\n";
	cout << i << "\n";
	cout << x << "\n";
	cout << str << "\n";

	//formatting2
	cout.unsetf(ios::hex|ios::showbase|
		ios::showpos);
	cout.setf(ios::dec|ios::scientific|
		ios::showpoint );
	cout << "\n10�i��, �Ȋw�Z�p�\��, �����_�ƌ㑱�[���\��\n";
	cout << i << "\n";
	cout << x << "\n";
	cout << str << "\n";

	return 0;
}