#include <iostream>
using namespace std;

int main() {
	//�K�v�ȕϐ���錾����
    char name[50];
    int score;
    char grade;
	//���O�Ɠ_������͂�����
    cout << "���O����͂��Ă�������:";
    cin >> name;
    cout << "�_������͂��Ă�������:";
    cin >> score;
    if (score < 0 || score > 100)
        grade = 'X';
    else if (score >= 90)
        grade = 'S';
    else if (score >= 80)
        grade = 'A';
    else if (score >= 70)
        grade = 'B';
    else if (score >= 60)
        grade = 'C';
    else
        grade = 'E';
    cout << name << "����̕]����" << grade << "�ł�\n";

    return 0;
}
