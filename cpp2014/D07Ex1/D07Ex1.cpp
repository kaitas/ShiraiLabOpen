#include <iostream>
using namespace std;
// �|�C���^���������Q�Ɖ��������g���ď�������

//void f(int *n) {
void f(int &n) {
	n = n * 2;
}

int main() {
	int i = 10;
	cout << " i = " << i << "\n";
//	f(&i);
	f(i);
	cout << " i = " << i << "\n";

	return 0;
}