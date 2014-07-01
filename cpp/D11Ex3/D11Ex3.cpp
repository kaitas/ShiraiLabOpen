#include <iostream>
using namespace std;

class Complex {
	double real;
	double imag;
public:
	Complex (double x=0, double y=0) {
		real = x;
		imag = y;
	}
	double getReal() {return real;}
	double getImag() {return imag;}
	void show(char *str) {
		cout << str << "( " << real << ", " << imag << "i)\n";
	}
	//�ۑ�1�̐錾
	Complex operator+ (Complex o);
	Complex operator- (Complex o);
	Complex operator* (Complex o);
	Complex operator/ (Complex o);
	//�ۑ�2�̐錾
	Complex operator+ (double d);
	Complex operator- (double d);
	Complex operator* (double d);
	Complex operator/ (double d);
	Complex operator- ();
	//�ۑ�3�̐錾
	friend Complex operator+ (double d, Complex o2);
	friend Complex operator- (double d, Complex o2);
	friend Complex operator* (double d, Complex o2);
	friend Complex operator/ (double d, Complex o2);

};
//�ۑ�1�̎���
Complex Complex::operator+ (Complex o) {
	double re, im;
	re = real + o.real;
	im = imag + o.imag;
	Complex ret(re, im);
	return ret;
}
Complex Complex::operator- (Complex o) {
	double re, im;
	re = real - o.real;
	im = imag - o.imag;
	Complex ret(re, im);
	return ret;
}
Complex Complex::operator* (Complex o) {
	double re, im;
	re = real*o.real - imag*o.imag;
	im = imag*o.real + real*o.imag;
	Complex ret(re, im);
	return ret;
}
Complex Complex::operator/ (Complex o) {
	double re, im, d;
	d = o.real*o.real + o.imag*o.imag;
	re = (real*o.real + imag*o.imag ) / d;
	im = (imag*o.real - real*o.imag ) / d;
	Complex ret(re, im);
	return ret;
}

//�ۑ�2�̎���
Complex Complex::operator+ (double d) {
	Complex ret(real + d , imag);
	return ret;
}
Complex Complex::operator- (double d) {
	Complex ret(real - d , imag);
	return ret;
}
Complex Complex::operator* (double d) {
	Complex ret(real * d , imag * d);
	return ret;
}
Complex Complex::operator/ (double d) {
	Complex ret(real / d , imag / d);
	return ret;
}
Complex Complex::operator- (){
	Complex ret(-real, -imag);
	return ret;
}

//�ۑ�3�̎���
Complex operator+ (double d, Complex o2) {
	double re;
	re = d + o2.real;
	Complex ret( re , o2.imag);
	return ret;
}
Complex operator- (double d, Complex o2) {
	double re;
	re = d - o2.real;
	Complex ret( re , -o2.imag);
	return ret;
}
Complex operator* (double d, Complex o2) {
	double re, im;
	re = d*o2.real;
	im = d*o2.imag;
	Complex ret( re , im);
	return ret;
}
Complex operator/ (double d, Complex o2) {
	double re, im, r;
	r = o2.real * o2.real + o2.imag * o2.imag;
	re = d*o2.real / r;
	im =-d*o2.imag / r;
	Complex ret( re , im);
	return ret;
}
int main() {
	Complex a(2.0, 1.2), b(3.2, 5.0);
	//�ۑ�1
	a.show(" a = ");
	b.show(" b = ");
	(a + b).show(" a + b = ");
	(a - b).show(" a - b = ");
	(a * b).show(" a * b = ");
	(a / b).show(" a / b = ");

	//�ۑ�2
	double x = 1.5f;
	Complex c;
	cout << "\n x = " << x << "\n";
	c = a + x;
	c.show(" a + x = ");
	c = a - x;
	c.show(" a - x = ");
	c = a * x;
	c.show(" a * x = ");
	c = a / x;
	c.show(" a / x = ");
	c = - a;
	c.show(" - a = ");

	//�ۑ�3
	cout << "\n";
	c = x + a;	c.show(" x + a = ");
	c = x - a;	c.show(" x - a = ");
	c = x * a;	c.show(" x * a = ");
	c = x / a;	c.show(" x / a = ");

	return 0;
}