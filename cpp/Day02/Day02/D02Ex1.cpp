#include <iostream>
using namespace std;


class Triangle {
    double a, b, c;
public:
    void set_a(double x);
    void set_b(double x);
    void set_c(double x);
    double get_a();
    double get_b();
    double get_c();
};

void Triangle::set_a(double x) {
    a = x;
}

void Triangle::set_b(double x) {
    b = x;
}

void Triangle::set_c(double x) {
    c = x;
}

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
    Triangle obj;
    double a, b, c;

    cout << "ŽOŠpŒ`‚Ì•Ó‚Ì’·‚³‚ð“ü—Í‚µ‚Ä‚­‚¾‚³‚¢:";
    cin >> a >> b >> c;

    obj.set_a(a);
    obj.set_b(b);
    obj.set_c(c);

    cout << "•Óa = " << obj.get_a() << "\n";
    cout << "•Ób = " << obj.get_b() << "\n";
    cout << "•Óc = " << obj.get_c() << "\n";

    return 0;
}
