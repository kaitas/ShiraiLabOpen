#include <iostream>
using namespace std;
//�����Ɋ֐���3�I�[�o�[���[�h�����ď���(p.29�Q��)
char max(char a, char b);
int max(int a, int b);
double max(double a, double b);
char max(char a, char b) {
	return a>b ? a: b;
}
int max(int a, int b) {
	return a>b ? a: b;
}
double max(double a, double b) {
	return a>b ? a: b;
}
int main() {
	//�e�X�g�p�̃f�[�^
	char c1 = 'a', c2 = 'b';
	int i1 = 10, i2 = 20;
	double d1 = 1.01, d2 = 0.99;

	cout << c1 << "��" << c2 << "���r���đ傫���̂�" 
		<< max(c1, c2) << "�ł�\n";

	cout << i1 << "��" << i2 << "���r���đ傫���̂�" 
		<< max(i1, i2) << "�ł�\n";

	cout << d1 << "��" << d2 << "���r���đ傫���̂�" 
		<< max(d1, d2) << "�ł�\n";

	return 0;
}
