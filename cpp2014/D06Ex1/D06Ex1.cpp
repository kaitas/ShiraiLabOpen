// Day 06 Ex 1
#include <iostream>
//#include <cstring>
using namespace std;

class StudentList {
	char name[5];
public:
	void set(char *n);
	void show();
};

void StudentList::set(char *n) {
	strcpy(name, n);
}

void StudentList::show() {
	cout << "�����F" << name << "\n";
}

int main() {
	StudentList s1, s2;//�f�[�^�I�u�W�F�N�g, ����͒��ڃA�N�Z�X���Ȃ����[��
	StudentList *ptr; //�Z�b�g�̂��߂Ɏg���I�u�W�F�N�g�A�|�C���^�����ւ��Ŏg��
	char nm[500];//����nm�͕s�v��������Ȃ�?���������̐����Ȃǂ��d�v�Ȃ̂ŕK�v
	//���ӁF���̃T���v���ł̓v���C�x�[�g�̃I�u�W�F�N�g���œn����������������Ȃ��������ł���
	//�z���錾���Ă��Ȃ����A�����񂪓n���Ă��܂��B
	//�Z�L�����e�B�Ȃǂ̍U���Ɏg����e�N�j�b�N�A�܂���݃o�O�𐶂�ł���Ƃ������ƁB

	ptr = &s1;
	cout << "1�l�ڂ̃f�[�^����͂��܂�\n";
	cout << "���O����͂��Ă�������:";
	cin >> nm;
	ptr->set(nm);
//	ptr->set("Hoge");

	ptr = &s2;
	cout << "2�l�ڂ̃f�[�^����͂��܂�\n";
	cout << "���O����͂��Ă�������:";
	cin >> nm;
	ptr->set(nm);

	ptr = &s1;
	cout << "1\t"; //�^�u�����Ă���
	ptr->show();
	ptr = &s2; //�|�C���^�����ւ�
	cout << "2\t";
	ptr->show();

	return 0;
}