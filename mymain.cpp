#include <iostream>
#include <iomanip>
using namespace std;
int mymain(int argc, char **argv)
{
    // печать переданных аргументов
    cout << "принято " << argc << " аргументов" << endl;
    if (argc > 0) for (int i = 0; i < argc; ++i) cout << i + 1 << ":" << argv[i] << endl;
    cout << "---------" << endl;
    for (int i = 0; i < 3; i++) {
        cout << i << " вывод@cout" << "\t" << "/forloop/rus/tab" << endl;
        cerr << hex << (i + 1) * 1111 << "@cerr" << "/forloop/hex" << endl;
        clog << setw(6) << setprecision(3) <<  float((i + 0.1) * 10 / 3.1) << "@clog" << "/forloop/fload" << endl;
    }
    int i = 9;
//    cin >> i;
    cout << ++i *i++ << "@cout" << endl;
    clog << --i *i-- << "@clog" << endl;
    return i;
}
