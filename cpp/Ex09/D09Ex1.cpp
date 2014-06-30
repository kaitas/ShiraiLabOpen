#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;

class strtype {
	char *p;
public:
	strtype(char *s);
	strtype(char *s, int n);
	char *get() {return p;}
};

strtype::strtype(char *s) {
	int l;
	l = strlen(s) + 1;
	p = new char [l];
	if (!p) {
		cout < "メモリ割り当てエラー\n";
		exit(1);
	}
	*p = '\0';
	strcat(p,s);
}

strtype::strtype(char *s, int n) {
	int l;
	l = strlen(s) * n + 1;
	p = new char [l];
	if (!p) {
		cout < "メモリ割り当てエラー\n";
		exit(1);
	}
	*p = '\0';
	for (int i=0; i < n ; i++ ){
		strcat(p,s);
	}
}

int main() {
	strtype s1 ("Good!");
	strtype s2("Good!", 5);
	cout << "s1:" << s1.get() << "\n";
	cout << "s2:" << s2.get() << "\n";

	return 0;
}