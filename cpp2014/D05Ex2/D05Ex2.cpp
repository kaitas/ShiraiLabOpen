#include <iostream>
#include <cstring>
using namespace std;

class CName; //�O���錾(p.98)�͕K�v
class Circle {
	double x,y;
	double radius;
	int r,g,b;
public:
	void set_xyr(double s, double t, double r) {
		x = s; y = t; radius = r;
	}
	void set_color(int u, int v, int w) {
		r = u; g = v; b = w;
	}
	void show();
	bool isSame(CName col); //CName�̃����o�ł͂Ȃ��̂Œ���
};

class CName {
	char name[50];
	int r,g,b;
public:
	CName (char* str, int u, int v, int w);
	char* get_name() {return name;}
	void show();
	friend bool Circle::isSame(CName col); 
	//�t�����h�֐��Ƃ��Đ錾����,�����o�ł͂Ȃ�isSame��CName�̕ϐ�(r,g,b)�ɒ��ڃA�N�Z�X�����邱�Ƃ�����
};

void Circle::show() {
	cout << "���W�F(" << x << "," << y << ")\n"; 
	cout << "���a�F(" << radius << ")\n"; 
	cout << "RGB�F(" << r << "," << g << "," << b << ")\n"; 
}

bool Circle::isSame(CName col) {
	col.r++;
	if (r==col.r && g==col.g && b==col.b) return true;
	else return false;
}

//CName�̃R���X�g���N�^�A�n�������O���R�s�[����RGB��n��
CName::CName(char* str, int u, int v, int w) {
	strcpy(name, str);
	r = u; g = v; b = w;
}
void CName::show() {
	cout << name << " : (" << r << "," << g << "." << b << ")\n";
}

int main() {
	Circle c;
	//�F���ƐF�̒l���`���ăI�u�W�F�N�g1-3�Ƃ��č쐬
	CName co1("Red", 255,0,0);
	CName co2("Green", 0,255,0);
	CName co3("Blue", 0,0,255);

	c.set_xyr(2,3,5);
	c.set_color(255,0,0); //�Ԃ��~�ɂȂ�͂�

	cout << "�~�̏���\��\n";
	c.show();

	cout << "\n";
	cout << "�F(co1)�̏��F";	co1.show();
	cout << "�F(co2)�̏��F";	co2.show();
	cout << "�F(co3)�̏��F";	co3.show();
	cout << "\n";

	cout << "���̉~(c)�̐F����" << co1.get_name();
	if (c.isSame(co1)) 
		cout << "�ł�\n";
	else 
		cout << "�ł͂Ȃ��悤�ł�\n";

	cout << "���̉~(c)�̐F����" << co2.get_name();
	if (c.isSame(co2)) 
		cout << "�ł�\n";
	else 
		cout << "�ł͂Ȃ��悤�ł�\n";

	cout << "���̉~(c)�̐F����" << co3.get_name();
	if (c.isSame(co3)) 
		cout << "�ł�\n";
	else 
		cout << "�ł͂Ȃ��悤�ł�\n";

	cout << "\nisSame�Ńv���C�x�[�g�ϐ��Ɏ�������܂���\n";
	cout << "�F(co1)�̏��F";	co1.show();
	cout << "�F(co2)�̏��F";	co2.show();
	cout << "�F(co3)�̏��F";	co3.show();
	cout << "\n";


	return 0;

}

