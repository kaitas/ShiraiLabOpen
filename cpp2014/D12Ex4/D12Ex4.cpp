#include <iostream>
using namespace std;

class base {
//protected:
	int a;
public:
	base (int n) { a = n; }
//	void seta(int n) {a = n;}
	int  geta() { return a;}
};

//�h���N���X
class derived : public base {
	int b;
public:
	derived(int n, int m) : base(n) {b = m;}
//	void setb(int n) { b = n;}
	int mul() { return b * geta(); }  //base.a �� protected �ł͂Ȃ��̂Œ��ڗ��p�s��
};

int main() {
	int a = 3, b = 5;
	cout << "a��" << a << "��ݒ肵�܂�\n";
	cout << "b��" << b << "��ݒ肵�܂�\n";
	derived ob(a,b); //�R���X�g���N�^�ɂ�鐶��
	cout << "a*b = " << ob.mul() << "\n";

	return 0;
}