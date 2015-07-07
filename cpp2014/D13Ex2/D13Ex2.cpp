//D13Ex2: 多重継承の基本2
#include <iostream>
using namespace std;

class Position{
	double x,y;
public:
	Position(double u, double v) { x= u; y = v;}
	void showpos();
};

class Color {
	int red, green, blue;
public:
	Color(int r, int g, int b) {red=r;green=g;blue=b;}
	void showcol();
};

class Graphic: private Position, private Color {
	int type;
	double width, height;
public:
	//多重継承
	Graphic(int t, double x, double y, 
		double w, double h, 
		int r, int g, int b) : Position( x, y), Color(r,g,b)  {
			type = t;
			if (w<0) width = 0;
			else width = w;
			if (h<0) height = 0;
			else height = h;
	}
	void show();
};

void Position::showpos() {
	cout << "(" << x << "," << y << ")";
}
void Color::showcol() {
	cout <<"("<<red<<","<<green<<","<<blue<<")"; 
}
void Graphic::show() {
	cout << "形状：";
	switch (type) {
		case 1:
			cout << "円\n"; break;
		case 2:
			cout << "四角形\n"; break;
		case 3: 
			cout << "三角形\n"; break;
		default: 
			cout << "不明\n";
	}
	cout << "位置："; showpos(); cout <<"\n幅：";
	cout << width << "\t高さ" << height << "\n色：";
	showcol(); cout << "\n"; 
}

int main() {
	Graphic ob1(1,3,4,1,2,255,0,0);
	Graphic ob2(2,5,6,7,8,0,255,0);
	Graphic ob3(3,-1,-2,10,10,0,0,255);
	cout << "ob1\n";
	ob1.show();
	cout << "ob2\n";
	ob2.show();
	cout << "ob3\n";
	ob3.show();
	return 0;
}