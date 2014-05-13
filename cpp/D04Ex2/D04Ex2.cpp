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
	cout << "辺a = " << a << "\n";
	cout << "辺b = " << b << "\n";
	cout << "辺c = " << c << "\n";
	cout << "面積 = " << area() << "\n";
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
	//ここで値を返す必要はない、void型関数ですし。
}

int main() {
	Triangle obj;//オブジェクト実体
	Triangle *p; //実験(p.58)
	p = &obj;
	
	double mul; //倍数
	obj.set_a(3.0);
	obj.set_b(4.0);
	obj.set_c(5.0);
	cout << "三角形\n"; obj.show();

	cout << "何倍にしますか？:"; cin >> mul;
	mul_tri(&obj, mul);
	cout << "三角形\n"; obj.show();
	return 0;
}