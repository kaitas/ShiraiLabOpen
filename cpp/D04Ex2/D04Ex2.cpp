#include <iostream>
#include <cmath>
using namespace std;

class Triangle {
	double a,b,c;
public:
	void set_a(double x) {a=x;}
	void set_b(double x) {b=x;}
	void set_c(double x) {c=x;}
	double get_a() {return a;}
	double get_b() {return b;}
	double get_c() {return c;}
	double area();
	void show();
};

double Triangle::area() {
	double s;
	s = (a+b+c) /2;
	return sqrt( s * (s-a) * (s-b) * (s-c));
}

void Triangle::show() {
	cout << "��a = " << a << "\n";
	cout << "��b = " << b << "\n";
	cout << "��c = " << c << "\n";
	cout << "�ʐ� = " << area() << "\n";
}

Triangle max_tri(Triangle o1, Triangle o2) {
	if (o1.area() > o2.area()) {
		return o1;
	} else {
		return o2;
	}
}

void mul_tri(Triangle *ob, double m) {
	ob->set_a( m * ob->get_a() ); 
	ob->set_b( m * ob->get_b() ); 
	ob->set_c( m * ob->get_c() ); 
	//�����Œl��Ԃ��K�v�͂Ȃ��Avoid�^�֐��ł����B
}

int main() {
	Triangle obj;//�I�u�W�F�N�g����
	Triangle *p; //����(p.58)
	p = &obj;
	
	double mul; //�{��
	obj.set_a(3.0);
	obj.set_b(4.0);
	obj.set_c(5.0);
	cout << "�O�p�`\n"; obj.show();

	cout << "���{�ɂ��܂����H:"; cin >> mul;
	mul_tri(&obj, mul);
	cout << "�O�p�`\n"; obj.show();
	return 0;
}