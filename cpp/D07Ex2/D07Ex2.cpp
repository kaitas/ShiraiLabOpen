//�l�n���A�|�C���^�n���A�Q�Ɠn��
#include <iostream>
using namespace std;

class Coord {
	double x, y, z;
public:
	void set(double u, double v, double w) {
		x = u;
		y = v;
		z = w;
	}
	void show();
	void setCoord(Coord ob);
};

void Coord::show() {
	cout << "(" << x << "," << y << "," << z << ")";
}

//�l�n��(�N���X���Ŏ������Ă݂�)
void Coord::setCoord(Coord ob) {
	double u,v,w;
	cout << "������3���͂��Ă�������:";
	cin >> u >> v >> w;
//	ob.set(u,v,w);
	set(u,v,w);
}


//�l�n��
void setCoord1 (Coord ob) {
	double u,v,w;
	cout << "������3���͂��Ă�������:";
	cin >> u >> v >> w;
	ob.set(u,v,w);
}

//�|�C���^�n��
void setCoord2 (Coord *ob) { //&�œn�����|�C���^��*�Ŏ󂯎��!!�����"�|�C���^�n��"�ƌĂ�
	double u,v,w;
	cout << "������3���͂��Ă�������:";
	cin >> u >> v >> w;
	ob->set(u,v,w); //�|�C���^�ł��������A���[���Z�q�Ŏ��̂ɃA�N�Z�X
}

//�Q�Ɠn��
void setCoord3 (Coord &ob) { //����œn����&�Ŏ󂯎��!!�����"�Q�Ɠn��"�ƌĂ�
	double u,v,w;
	cout << "������3���͂��Ă�������:";
	cin >> u >> v >> w;
	ob.set(u,v,w); //�Q�Ɖ�����
}


int main() {
	Coord obj;
	obj.set(1.1, 2.3, 5.5);
	cout << "�I�u�W�F�N�g�̏����l:";
	obj.show();
	cout << "\n\n";
	cout << "�l�n���Ŋ֐��ɃI�u�W�F�N�g��n���܂�\n";
	setCoord1(obj);
//	obj.setCoord(obj);
	cout << "�I�u�W�F�N�g�̒l�F";
	obj.show(); //���ۂɂ͂����Œl�͕ς��Ȃ�
	cout << "\n\n";

	cout << "�|�C���^�n���Ŋ֐��ɃI�u�W�F�N�g��n���܂�\n";
	setCoord2(&obj);
	cout << "�I�u�W�F�N�g�̒l�F";
	obj.show(); //�l�͕ς��H��Yes
	cout << "\n\n";

	cout << "�Q�Ɠn���Ŋ֐��ɃI�u�W�F�N�g��n���܂�\n";
	setCoord3(obj);
	cout << "�I�u�W�F�N�g�̒l�F";
	obj.show(); //�l�͕ς��H
	cout << "\n\n";

	return 0;
}