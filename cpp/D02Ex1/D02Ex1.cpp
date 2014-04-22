#include <iostream>
using namespace std;
class Triangle {
    double a, b, c;
public:
    void set_a(double x);	//辺aをセットする関数
    void set_b(double x);	//辺bをセットする関数
    void set_c(double x);	//辺cをセットする関数
    double get_a();			//辺aの値を得る関数
    double get_b();			//辺bの値を得る関数
    double get_c();			//辺cの値を得る関数
};
//関数の実装
void Triangle::set_a(double x) {
    a = x;
}
void Triangle::set_b(double x) {
    b = x;
}
void Triangle::set_c(double x) {
    c = x;
}
//メンバ変数アクセス関数の実装
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
    Triangle obj; //クラスTriangleの実体objを得る
    double a, b, c; //これは単なるローカル変数
    cout << "三角形の辺の長さを入力してください:";
    cin >> a >> b >> c;
	//入力した値をクラスオブジェクトに設定
	obj.set_a(a);
    obj.set_b(b);
    obj.set_c(c);
	//設定した値が正しく格納されているか、メソッドを使って確認
    cout << "辺a = " << obj.get_a() << "\n";
    cout << "辺b = " << obj.get_b() << "\n";
    cout << "辺c = " << obj.get_c() << "\n";
    return 0;
}
