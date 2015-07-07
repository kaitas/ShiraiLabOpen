#include <iostream>
using namespace std;

class Coord {
	double x;
	double y;
	double z;
public:
	Coord(double s, double t, double u) {
		x = s; y = t; z = u;
	}
	double get_x() {return x;}
	double get_y() {return y;}
	double get_z() {return z;}
	void show();
};

void Coord::show() {
	cout << "(" << x << "," << y << "," << z << "," << ")";
}

int main () {
	Coord ob[4] = {
		Coord(1,2,3),
		Coord(4,5,6),
		Coord(7,8,9),
		Coord(-1,2,-3)
	};
	for (int i=0; i<4; i++) {
		cout << "ob[" << i << "]=";
		ob[i].show();
		cout << '\n';
	}
	return 0;
}