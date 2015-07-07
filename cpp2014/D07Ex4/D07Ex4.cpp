//トランプのシャッフル
//参照渡しを用いた値の交換

#include <iostream>
#include <cstdlib>
using namespace std;

enum suit {SPADE, HEART, CLUB, DIAMOND};

class PCard {
	enum suit suit;
	int number;
public:
	void set (enum suit st, int nm) {
		suit = st;
		number = nm;
	}
	void show();
};

void PCard::show() {
	switch(suit) {
	case SPADE:
		cout << "スペードの";	break;
	case HEART:
		cout << "ハートの";		break;
	case CLUB:
		cout << "クラブの";		break;
	case DIAMOND:
		cout << "ダイヤの";		break;
	}
	cout << number << "\n";
}

//
void swap(PCard &ob1, PCard &ob2) {
	ob1.show(); cout << "と";  ob2.show(); cout << "をswap\n";
	PCard ob;
	ob = ob1;
	ob1 = ob2;
	ob2 = ob;
}
int main() {
	PCard deck[52];
	//カード初期化
	for (int i = 0; i < 4; i++ ) {
		for (int j = 1; j <=13; j++) {
			switch(i) {
			case 0:
				deck[i*13+j-1].set(SPADE, j);				break;
			case 1:
				deck[i*13+j-1].set(HEART, j);				break;
			case 2:
				deck[i*13+j-1].set(CLUB, j);				break;
			case 3:
				deck[i*13+j-1].set(DIAMOND, j);				break;
			}
		}
	}
	//初期化したカードを表示してみる
	for (int i=0; i< 52; i++) {
		deck[i].show();
	}

	cout << "\nシャッフルします\n\n";
	for (int i=0; i<3; i++) {
		int a,b;
		//52で割った余り（0-51）
		a = rand() % 52;
		b = rand() % 52;
		swap(deck[a],deck[b]);
	}
	//swapしたカードを表示してみる
	for (int i=0; i< 52; i++) {
		deck[i].show();
	}
	return 0;
}

