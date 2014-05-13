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
	cout << "•Óa = " << a << "\n";
	cout << "•Ób = " << b << "\n";
	cout << "•Óc = " << c << "\n";
	cout << "–ÊÏ = " << area() << "\n";
}

Triangle max_tri(Triangle o1, Triangle o2) {
	if (o1.area() > o2.area()) {
		return o1;
	} else {
		return o2;
	}
}

int main() {
	Triangle obj1, obj2, obj3;
	obj1.set_a(3.0);
	obj1.set_b(4.0);
	obj1.set_c(5.0);
	obj2.set_a(5.0);
	obj2.set_b(12.0);
	obj2.set_c(13.0);

	cout << "ŽOŠpŒ`\n"; obj1.show();
	cout << "ŽOŠpŒ`\n"; obj2.show();

	obj3 = max_tri(obj1,obj2);
	cout << "‘å‚«‚¢ŽOŠpŒ`\n";
	obj3.show();
	return 0;
}