// (�K�{)�Q�Ƃ̕��K�A���ёւ��A���S���Y���̕��K

#include <iostream>
using namespace std;

void order (int &a, int &b, int &c) {
	int t;
	if (b < a) {
		t = a;
		a = b;
		b = t;
	}
	if (c < a) {
		t = a;
		a = c;
		c = t;
	}
	if (c < b) {
		t = b;
		b = c;
		c = t;
	}
}

int main() {
	int x, y, z;

	cout << "Input x:";
	cin >> x;
	cout << "Input y:";
	cin >> y;
	cout << "Input z:";
	cin >> z;
	order(x,y,z);
	cout << "x:" << x << ", y: " << y << ", z:" << z << "\n";
	return 0;
}

