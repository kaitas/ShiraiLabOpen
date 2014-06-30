//デフォルト引数

#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;

class strtype {
	char *p;
public:
//	strtype(char *s);  //この形式で書く必要がなくなった
	strtype(char *s, int n = 1); //デフォルト引数を使用している
	strtype(const strtype &o); //コピーコンストラクタ
	~strtype() { delete [] p; } //デストラクタ
	char *get() {return p;}
};

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
	char c, s[100];
	int n;
	cout << "文字列 s1 を設定します。\n文字列を入力してください：";
	cin >> s;
	strtype s1(s);  //デフォルト引数のおかげで繰り返しは(n=1)

	cout << "繰り返し文字列 s2 を設定します。\n文字列を入力してください：";
	cin >> s;
	cout << "繰り返し回数を入力してください：";
	cin >> n;
	strtype s2(s,n);

	cout << "文字列 s1 と s2 に含まれる ある文字 の数を数えます。\n一文字入力してください：";
	cin >> c;

	cout << "s1の "<<c<<" の数:" << ccount(s1, c) << "\n";
	cout << "s2の "<<c<<" の数:" << ccount(s2, c) << "\n";

	cout << "s1:" << s1.get() << "\n";
	cout << "s2:" << s2.get() << "\n";

	return 0;
}