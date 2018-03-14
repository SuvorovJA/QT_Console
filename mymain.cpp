#include"mymain.h"

#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
int mymain(int argc, char **argv)
{
    // печать переданных аргументов
    cout << "принято " << argc << " аргументов" << endl;
    if (argc > 0) for (int i = 0; i < argc; ++i) cout << i + 1 << ":" << argv[i] << endl;

    cout << "--------- демонстрация вывода ---------" << endl;
    for (int i = 0; i < 3; i++) {
        cout << i << " вывод@cout" << "\t" << "/forloop/rus/tab" << endl;
        cerr << hex << (i + 1) * 1111 << "@cerr" << "/forloop/hex" << endl;
        clog << setw(6) << setprecision(3) <<  float((i + 0.1) * 10 / 3.1) << "@clog" << "/forloop/fload" << endl;
    }

    cout << "--------- демонстрация ввода значений через dialog ---------" << endl;
    int i = 9;
    DialogBoxSS *dbox = MainWindow::dbholder; // лишний мэджик :( TODO вынести за пределы main
    cout << dbox->getString("Введите что-нибудь...") << endl;
    i = stoi(dbox->getString("Введите число:")); // c конвертацией в int через stoi
    cout << ++i *i++ << "@cout" << endl;
    clog << --i *i-- << "@clog" << endl;
    stringstream ss;  // конвертация через поток
    ss << dbox->getString("Введите дробное число и целое число:");
    double dv = 0;
    i = 0;
    ss >> dv >> i ;
    cout << "Вы ввели \"" << setw(6) << setprecision(3) << dv << "\" и \"" << i << "\"" << endl;


    //  cin >> i; // не работает пока
    return i;
}
