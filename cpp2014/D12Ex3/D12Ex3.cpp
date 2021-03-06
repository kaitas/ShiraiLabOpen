#include <iostream>
using namespace std;

class base {
protected:
	int a;
public:
	void seta(int n) {a = n;}
	int  geta() { return a;}
};

class derived : public base {
	int b;
public:
	void setb(int n) { b = n;}
	int mul() { return b * a; }  //base.a が protected なので直接利用できる
};

int main() {
	derived ob;
	int a = 3, b = 5;
	cout << "aに" << a << "を設定します\n";
	ob.seta(a);
	cout << "bに" << b << "を設定します\n";
	ob.setb(b);
	cout << "a*b = " << ob.mul() << "\n";
	cout << "aに" << 6 << "をseta(6)で設定します\n";
	ob.seta(6);
	cout << "a*b = " << ob.mul() << "\n";

	return 0;
}