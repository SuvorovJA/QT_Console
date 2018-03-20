#include "exchangeproto.h"
#include <fstream>
#include <string>
#include <iostream>
using namespace std;
ExchangeProto::ExchangeProto()
{

    cout << "sizeof(ExchangePackMessageAlignmentOneByte)=" << sizeof(ExchangePackMessageAlignmentOneByte) << endl;
    cout << "sizeof(ExchangePackMessageNonAlignment)=" << sizeof(ExchangePackMessageNonAlignment) << endl << endl;
    ifstream readfile;
    readfile.open("../QT_Console__Lab7H/exchangeproto.h");
    if (readfile) {
        cout << "content of \"exchangeproto.h\":" << endl << endl;
        string line;
        while (!readfile.eof()) {
            getline(readfile, line);
            cout << line << endl;
        }
        readfile.close();
    }
}
