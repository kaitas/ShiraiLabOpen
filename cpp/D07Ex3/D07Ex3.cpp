//�|�C���^�̕��K
#include <iostream>
using namespace std;

class samp {
	int a, b;
public: 
	samp (int n, int m) {
		a = n;
		b = m;
	}
	void show() {
		cout << "(" << a << "," << b << ")\n"; 
	}
};

int main () {
	//���s���ʂ��Q�l�ɔz��I�u�W�F�N�g��������
	samp ob[3][2] = {
		samp(1,
		2), samp(2,3),
		samp(3,4), samp(4,5),
		samp(5,6), samp(6,7)
	};
	samp *p;
	p = (samp *) ob; //���̃|�C���^�n���ł͔z�񂪓n��킯�ł͂Ȃ��A�z��̐擪�݂̂�p�ɓ���
	for (int i = 0; i <6; i++) {
		//�|�C���^���g���ďo��
		p->show();
		//�|�C���^�̎����A�h���X��1���ɂ���
		p++;
	}
	return 0;
}