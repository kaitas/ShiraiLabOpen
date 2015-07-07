#include <iostream>
using namespace std;

int main() {
	//必要な変数を宣言する
    char name[50];
    int score;
    char grade;
	//名前と点数を入力させる
    cout << "名前を入力してください:";
    cin >> name;
    cout << "点数を入力してください:";
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
    cout << name << "さんの評価は" << grade << "です\n";

    return 0;
}
