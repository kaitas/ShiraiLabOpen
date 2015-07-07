//�f�t�H���g����

#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;

class strtype {
	char *p;
public:
//	strtype(char *s);  //���̌`���ŏ����K�v���Ȃ��Ȃ���
	strtype(char *s, int n = 1); //�f�t�H���g�������g�p���Ă���
	strtype(const strtype &o); //�R�s�[�R���X�g���N�^
	~strtype() { delete [] p; } //�f�X�g���N�^
	char *get() {return p;}
};

strtype::strtype(char *s, int n) {
	int l;
	l = strlen(s) * n + 1;
	p = new char [l];
	if (!p) {
		cout < "���������蓖�ăG���[\n";
		exit(1);
	}
	*p = '\0';
	for (int i=0; i < n ; i++ ){
		strcat(p,s);
	}
}

//�R�s�[�R���X�g���N�^�̎���
strtype::strtype(const strtype &o) {
	int l;
	l = strlen(o.p) + 1; //����p�͂ǂ���p??
	p = new char [l];
	if (!p) {
		cout < "���������蓖�ăG���[\n";
		exit(1);
	}
	*p = '\0';
	strcat(p, o.p);
}

int ccount(strtype x, char c) {
	int count = 0;
	char *p;
	p = x.get();
	while (*p) {
		if (*p==c) count++;
		p++;
	}
	return count;
}

int main() {
	char c, s[100];
	int n;
	cout << "������ s1 ��ݒ肵�܂��B\n���������͂��Ă��������F";
	cin >> s;
	strtype s1(s);  //�f�t�H���g�����̂������ŌJ��Ԃ���(n=1)

	cout << "�J��Ԃ������� s2 ��ݒ肵�܂��B\n���������͂��Ă��������F";
	cin >> s;
	cout << "�J��Ԃ��񐔂���͂��Ă��������F";
	cin >> n;
	strtype s2(s,n);

	cout << "������ s1 �� s2 �Ɋ܂܂�� ���镶�� �̐��𐔂��܂��B\n�ꕶ�����͂��Ă��������F";
	cin >> c;

	cout << "s1�� "<<c<<" �̐�:" << ccount(s1, c) << "\n";
	cout << "s2�� "<<c<<" �̐�:" << ccount(s2, c) << "\n";

	cout << "s1:" << s1.get() << "\n";
	cout << "s2:" << s2.get() << "\n";

	return 0;
}