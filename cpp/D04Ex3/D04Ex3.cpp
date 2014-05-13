#include <iostream>
#include <cmath>
using namespace std;
class Coord {
	double x; double y; double z;
public:
	void set_x(double u) {x=u;}
	void set_y(double v) {y=v;}
	void set_z(double w) {z=w;}
	double get_x() {return x;}
	double get_y() {return y;}
	double get_z() {return z;}
	void show();
};

void Coord::show() {
	cout << "(" << x << "," << y << "," << z << ")";
}

//関数のプロトタイプ（今回、実装はMainの下に書いています）
Coord mid_Coord(Coord o1, Coord o2);
double dist_Coord(Coord o1, Coord o2);

int main (){
	Coord p1,p2,p3;
	p1.set_x(1.0);  p1.set_y(6.5); p1.set_z(-10.0);
	p2.set_x(-1.0); p2.set_y(1.5); p2.set_z(4.0);
	//cout割愛ね
	p3 = mid_Coord(p1,p2);
	cout << "AB間の中点は";
	p3.show();
	cout <<"AB間の距離は"<<dist_Coord(p1,p2);
	return 0;
}

//関数の実装をmainの下に書く記法
//mid_Coord(), dist_Coord()の実装
Coord mid_Coord(Coord o1, Coord o2) {
	Coord m; //中点
	m.set_x( ( o1.get_x() + o2.get_x() ) / 2 );
	m.set_y( ( o1.get_y() + o2.get_y() ) / 2 );
	m.set_z( ( o1.get_z() + o2.get_z() ) / 2 );
	return m;
}

double dist_Coord(Coord o1, Coord o2) {
	//1行で書く！
	return sqrt(
 		 (o1.get_x()- o2.get_x())*(o1.get_x()- o2.get_x()) +
		 (o1.get_y()- o2.get_y())*(o1.get_y()- o2.get_y()) +
		 (o1.get_z()- o2.get_z())*(o1.get_z()- o2.get_z()) 
		);
}

