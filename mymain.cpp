#include"mymain.h"
#include"contents.h"
#include"exchangeproto.h"
#include"rational.h"

#include <iostream>
//#include <iomanip>
//#include <sstream>
using namespace std;
int mymain(int argc, char **argv)
{

    switch (argc) {
    case 0:
        new Contents();
        break;
    case 1:
        char num;
        num = argv[0][0];
        switch (num) {
        case '2':
            cout << "2:" << endl;
            Rational::Test();
            break;
        case '3':
            cout << "3:" << endl;
            cout << "мне кажется это всё проделано в 4";
            break;
        case '4':
            cout << "4:" << endl;
            new ExchangeProto();
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return 0;
}
