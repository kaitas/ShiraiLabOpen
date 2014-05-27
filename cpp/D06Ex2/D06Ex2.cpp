// Day 06 Ex 2
#include <iostream>
//#include <cstring>
using namespace std;

class StudentList {
	char name[5];  //実験のためあえて5にしてある
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
	cout << "氏名：" << name;
	cout << "\t学籍番号" << id ;  //タブを使って成形します
	cout << "\t学年"<< grade << "\n";
}

int main() {
	StudentList slist[5];//データオブジェクト, 今回は直接アクセスしないルール
	char nm[50];
	int id, grd; //入力のために使う変数
	StudentList *ptr; //セットのために使うオブジェクト、ポインタ差し替えで使う

	ptr = slist;
	for (int i=0; i<5; i++) {
		cout << i+1 << "人目のデータを入力します\n";
		cout << "名前を入力してください:";
		cin >> nm;
		cout << "学籍番号を入力してください:";
		cin >> id;
		cout << "学年を入力してください:";
		cin >> grd;
		ptr->set(nm,id,grd);
		ptr++;    // ポインタをデータ1個分インクリメント（加算）する＝＞次のデータ先頭へ！
	}
	//表示部
	ptr = slist; //ポインタをオブジェクト配列先頭に戻す
	for (int i=0; i<5; i++) {
		cout << "[" << i+1 << "]\t";
		ptr->show();
		ptr++;   //ここでもポインタをインクリメントして次のデータの先頭に移動します
	}

	return 0;
}