#include <iostream>
using namespace std;

class base {
	int a;
public:
	void seta(int n) {a = n;}
	int  geta() { return a;}
};

class derived : public base {
	int b;
public:
	void setb(int n) { b = n;}
	int mul() { return b * geta(); }
};

int main() {
	derived ob;
	int a = 3, b = 5;
	cout << "a��" << a << "��ݒ肵�܂�\n";
	ob.seta(a);
	cout << "b��" << b << "��ݒ肵�܂�\n";
	ob.setb(b);
	cout << "a*b = " << ob.mul() << "\n";

	return 0;
}