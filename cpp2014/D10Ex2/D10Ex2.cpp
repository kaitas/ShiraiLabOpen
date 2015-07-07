// クラスのオブジェクト作成とメンバ関数へのアクセス復習、式を記述する練習
#include <iostream>
using namespace std;
class Complex {
	double real;
	double imag;
public:
	Complex(double x=0, double y=0) {
		real = x;
		imag = y;
	}
	double getReal() { return real; }
	double getImag() { return imag; }
	void show(char *str) {
		cout << str << "(" << real << ", " << imag << "i)\n";
	}
};

int main() {
	Complex a(2.0, 1.2), b(3.2, 5.0);
	double re, im, d;

	a.show("a = ");
	b.show("b = ");

	//たしざん
	re = a.getReal() + b.getReal();
	im = a.getImag() + b.getImag();
	Complex(re, im).show(" a + b = ");
	//引き算も同様に…
	re = a.getReal() - b.getReal();
	im = a.getImag() - b.getImag();
	Complex(re, im).show(" a - b = ");

	//掛け算
	re = a.getReal() * b.getReal()
	   - a.getImag() * b.getImag();
	im = a.getImag() * b.getReal()
	   + a.getReal() * b.getImag();
	Complex(re, im).show(" a * b = ");

	//除算
	d = b.getReal() * b.getReal() + b.getImag() * b.getImag();
	re = ( a.getReal() * b.getReal() + a.getImag() * b.getImag() ) / d;
	im = ( a.getImag() * b.getReal() - a.getReal() * b.getImag() ) / d;
	Complex(re, im).show(" a / b = ");

	return 0;
}