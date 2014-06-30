//コピーコンストラクタ

#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;

class strtype {
	char *p;
public:
	strtype(char *s);
	strtype(char *s, int n);
	strtype(const strtype &o); //コピーコンストラクタ
	~strtype() { delete [] p; } //デストラクタ
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

//コピーコンストラクタの実装
strtype::strtype(const strtype &o) {
	int l;
	l = strlen(o.p) + 1; //このpはどこのp??
	p = new char [l];
	if (!p) {
		cout < "メモリ割り当てエラー\n";
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
	strtype s1 ("Good!");
	strtype s2("Good!", 5);

	cout << "s1の o の数:" << ccount(s1,'o') << "\n";
	cout << "s2の o の数:" << ccount(s2,'o') << "\n";

	cout << "s1:" << s1.get() << "\n";
	cout << "s2:" << s2.get() << "\n";

	return 0;
}