#include <iostream>
using namespace std;

char max(char a, char b);
int max(int a, int b);
double max(double a, double b);

int main() {
	char c1 = 'a', c2 = 'b';
	int i1 = 10, i2 = 20;
	double d1 = 1.01, d2 = 0.99;

	cout << c1 << "‚Æ" << c2 << "‚ð”äŠr‚µ‚Ä‘å‚«‚¢‚Ì‚Í" << max(c1, c2) << "‚Å‚·\n";
	cout << i1 << "‚Æ" << i2 << "‚ð”äŠr‚µ‚Ä‘å‚«‚¢‚Ì‚Í" << max(i1, i2) << "‚Å‚·\n";
	cout << d1 << "‚Æ" << d2 << "‚ð”äŠr‚µ‚Ä‘å‚«‚¢‚Ì‚Í" << max(d1, d2) << "‚Å‚·\n";

	return 0;
}

char max(char a, char b) {
	return a>b ? a: b;
}

int max(int a, int b) {
	return a>b ? a: b;
}

double max(double a, double b) {
	return a>b ? a: b;
}
