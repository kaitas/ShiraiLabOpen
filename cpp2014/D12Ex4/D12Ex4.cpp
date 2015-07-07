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

//派生クラス
class derived : public base {
	int b;
public:
	derived(int n, int m) : base(n) {b = m;}
//	void setb(int n) { b = n;}
	int mul() { return b * geta(); }  //base.a は protected ではないので直接利用不可
};

int main() {
	int a = 3, b = 5;
	cout << "aに" << a << "を設定します\n";
	cout << "bに" << b << "を設定します\n";
	derived ob(a,b); //コンストラクタによる生成
	cout << "a*b = " << ob.mul() << "\n";

	return 0;
}