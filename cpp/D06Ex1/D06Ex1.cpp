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
	cout << "氏名：" << name << "\n";
}

int main() {
	StudentList s1, s2;//データオブジェクト, 今回は直接アクセスしないルール
	StudentList *ptr; //セットのために使うオブジェクト、ポインタ差し替えで使う
	char nm[500];//このnmは不要かもしれない?→文字数の制限などが重要なので必要
	//注意：このサンプルではプライベートのオブジェクト内で渡した文字列よりも少ない文字数でしか
	//配列を宣言していないが、文字列が渡せてしまう。
	//セキュリティなどの攻撃に使えるテクニック、つまり潜在バグを生んでいるということ。

	ptr = &s1;
	cout << "1人目のデータを入力します\n";
	cout << "名前を入力してください:";
	cin >> nm;
	ptr->set(nm);
//	ptr->set("Hoge");

	ptr = &s2;
	cout << "2人目のデータを入力します\n";
	cout << "名前を入力してください:";
	cin >> nm;
	ptr->set(nm);

	ptr = &s1;
	cout << "1\t"; //タブを入れている
	ptr->show();
	ptr = &s2; //ポインタ差し替え
	cout << "2\t";
	ptr->show();

	return 0;
}