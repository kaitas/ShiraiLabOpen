#include <iostream>
using namespace std;

int main() {
    char name[50];
    int score;
    char grade;

    cout << "–¼‘O‚ð“ü—Í‚µ‚Ä‚­‚¾‚³‚¢:";
    cin >> name;
    cout << "“_”‚ð“ü—Í‚µ‚Ä‚­‚¾‚³‚¢:";
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
    cout << name << "‚³‚ñ‚Ì•]‰¿‚Í" << grade << "‚Å‚·\n";

    return 0;
}
