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

// XYZ�͎��͂ŏ����Ă݂悤
class XYZ : public XY {
	double z;
public:
	XYZ() { z = 0;} // x,y�͌p�����̃N���X�Œ�`����Ă���
	XYZ(double u, double v, double w) : XY (u,v) {
		z = w; //�p�������ł͐ݒ�ł��Ȃ������o���������΂悢
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
