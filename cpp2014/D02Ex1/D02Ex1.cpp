#include <iostream>
using namespace std;
class Triangle {
    double a, b, c;
public:
    void set_a(double x);	//��a���Z�b�g����֐�
    void set_b(double x);	//��b���Z�b�g����֐�
    void set_c(double x);	//��c���Z�b�g����֐�
    double get_a();			//��a�̒l�𓾂�֐�
    double get_b();			//��b�̒l�𓾂�֐�
    double get_c();			//��c�̒l�𓾂�֐�
};
//�֐��̎���
void Triangle::set_a(double x) {
    a = x;
}
void Triangle::set_b(double x) {
    b = x;
}
void Triangle::set_c(double x) {
    c = x;
}
//�����o�ϐ��A�N�Z�X�֐��̎���
double Triangle::get_a() {
    return a;
}
double Triangle::get_b() {
    return b;
}
double Triangle::get_c() {
    return c;
}
int main() {
    Triangle obj; //�N���XTriangle�̎���obj�𓾂�
    double a, b, c; //����͒P�Ȃ郍�[�J���ϐ�
    cout << "�O�p�`�̕ӂ̒�������͂��Ă�������:";
    cin >> a >> b >> c;
	//���͂����l���N���X�I�u�W�F�N�g�ɐݒ�
	obj.set_a(a);
    obj.set_b(b);
    obj.set_c(c);
	//�ݒ肵���l���������i�[����Ă��邩�A���\�b�h���g���Ċm�F
    cout << "��a = " << obj.get_a() << "\n";
    cout << "��b = " << obj.get_b() << "\n";
    cout << "��c = " << obj.get_c() << "\n";
    return 0;
}
