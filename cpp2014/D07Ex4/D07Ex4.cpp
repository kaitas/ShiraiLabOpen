//�g�����v�̃V���b�t��
//�Q�Ɠn����p�����l�̌���

#include <iostream>
#include <cstdlib>
using namespace std;

enum suit {SPADE, HEART, CLUB, DIAMOND};

class PCard {
	enum suit suit;
	int number;
public:
	void set (enum suit st, int nm) {
		suit = st;
		number = nm;
	}
	void show();
};

void PCard::show() {
	switch(suit) {
	case SPADE:
		cout << "�X�y�[�h��";	break;
	case HEART:
		cout << "�n�[�g��";		break;
	case CLUB:
		cout << "�N���u��";		break;
	case DIAMOND:
		cout << "�_�C����";		break;
	}
	cout << number << "\n";
}

//
void swap(PCard &ob1, PCard &ob2) {
	ob1.show(); cout << "��";  ob2.show(); cout << "��swap\n";
	PCard ob;
	ob = ob1;
	ob1 = ob2;
	ob2 = ob;
}
int main() {
	PCard deck[52];
	//�J�[�h������
	for (int i = 0; i < 4; i++ ) {
		for (int j = 1; j <=13; j++) {
			switch(i) {
			case 0:
				deck[i*13+j-1].set(SPADE, j);				break;
			case 1:
				deck[i*13+j-1].set(HEART, j);				break;
			case 2:
				deck[i*13+j-1].set(CLUB, j);				break;
			case 3:
				deck[i*13+j-1].set(DIAMOND, j);				break;
			}
		}
	}
	//�����������J�[�h��\�����Ă݂�
	for (int i=0; i< 52; i++) {
		deck[i].show();
	}

	cout << "\n�V���b�t�����܂�\n\n";
	for (int i=0; i<3; i++) {
		int a,b;
		//52�Ŋ������]��i0-51�j
		a = rand() % 52;
		b = rand() % 52;
		swap(deck[a],deck[b]);
	}
	//swap�����J�[�h��\�����Ă݂�
	for (int i=0; i< 52; i++) {
		deck[i].show();
	}
	return 0;
}

