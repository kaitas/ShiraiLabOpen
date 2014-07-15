#include <iostream>
using namespace std;

class X {
	double x;
public:
	X() { x = 0; }
	X(double u) { x = u; }
	double get_x() { return x;}
};

class XY : public X {
	double y;
public:
	XY() { y = 0; }
	XY(double u, double v) : X(u) { y = v; }
	double get_y() { return y;}
};

// XYZは自力で書いてみよう
class XYZ : public XY {
	double z;
public:
	XYZ() { z = 0;} // x,yは継承元のクラスで定義されている
	XYZ(double u, double v, double w) : XY (u,v) {
		z = w; //継承だけでは設定できないメンバだけ書けばよい
	}
	double get_z() {return z;}
};

int main() {
	XYZ ob1, ob2(2,3,4);
	cout << "ob1 =(";
	cout << ob1.get_x() << ",";
	cout << ob1.get_y() << ",";
	cout << ob1.get_z() << ")\n";

	cout << "ob2 =(";
	cout << ob2.get_x() << ",";
	cout << ob2.get_y() << ",";
	cout << ob2.get_z() << ")\n";

	return 0;
}
