// Day 06 Ex 2
#include <iostream>
//#include <cstring>
using namespace std;

class StudentList {
	char name[5];  //�����̂��߂�����5�ɂ��Ă���
	int id;	int grade;
public:
	void set(char *n, int i, int g);
	void show();
};

void StudentList::set(char *n, int iId, int iGrade) {
	strcpy(name, n);
	id = iId;
	grade = iGrade;
}

void StudentList::show() {
	cout << "�����F" << name;
	cout << "\t�w�Дԍ�" << id ;  //�^�u���g���Đ��`���܂�
	cout << "\t�w�N"<< grade << "\n";
}

int main() {
	StudentList slist[5];//�f�[�^�I�u�W�F�N�g, ����͒��ڃA�N�Z�X���Ȃ����[��
	char nm[50];
	int id, grd; //���͂̂��߂Ɏg���ϐ�
	StudentList *ptr; //�Z�b�g�̂��߂Ɏg���I�u�W�F�N�g�A�|�C���^�����ւ��Ŏg��

	ptr = slist;
	for (int i=0; i<5; i++) {
		cout << i+1 << "�l�ڂ̃f�[�^����͂��܂�\n";
		cout << "���O����͂��Ă�������:";
		cin >> nm;
		cout << "�w�Дԍ�����͂��Ă�������:";
		cin >> id;
		cout << "�w�N����͂��Ă�������:";
		cin >> grd;
		ptr->set(nm,id,grd);
		ptr++;    // �|�C���^���f�[�^1���C���N�������g�i���Z�j���遁�����̃f�[�^�擪�ցI
	}
	//�\����
	ptr = slist; //�|�C���^���I�u�W�F�N�g�z��擪�ɖ߂�
	for (int i=0; i<5; i++) {
		cout << "[" << i+1 << "]\t";
		ptr->show();
		ptr++;   //�����ł��|�C���^���C���N�������g���Ď��̃f�[�^�̐擪�Ɉړ����܂�
	}

	return 0;
}