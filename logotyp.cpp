#include "logotyp.h"

#include <iostream>
using namespace std;

LogoTyp::LogoTyp()
{
    this->size = 13;
    this->data = new long unsigned int[this->size];
    this->data[0] = 90071992547409925;
    this->data[1] =  70432084436128;
    this->data[2] = 1073138544995764;
    this->data[3] = 8683958032639281;
    this->data[4] = 69343189903095025;
    this->data[5] = 32390803116912051;
    this->data[6] = 33600059748968088;
    this->data[7] = 33454924214100480;
    this->data[8] = 33600059748968599;
    this->data[9] = 31814453301767601;
    this->data[10] = 31814479390721175;
    this->data[11] = 132607085507510272;
    this->data[12] = 90071992547409925;
}

void LogoTyp::print()
{
    unsigned long int v = 0;
    for (int i = 0; i < this->size; ++i) {
        v = *(this->data + i);
        for (int c = 0; c < 19; ++c) {
            int code = v & 7;
            cout << this->decode(code);
            v = v >> 3;
        }
        cout << endl;
    }
}

char LogoTyp::decode(int in)
{
    switch (in) {
    case 0:
        return ' ';
    case 1:
        return '|';
    case 2:
        return '-';
    case 3:
        return '\\';
    case 4:
        return '/';
    case 5:
        return '*';
    case 6:
        return '.';
    case 7:
        return '+';
    default:
        return ' ';
        break;
    }
}
