#include "exchangeproto.h"
#include <fstream>
#include <string>
#include <iostream>
using namespace std;
ExchangeProto::ExchangeProto()
{

    cout << "sizeof(ExchangePackMessageAlignmentOneByte)=" << sizeof(ExchangePackMessageAlignmentOneByte) << endl;
    cout << "sizeof(ExchangePackMessageNonAlignment)=" << sizeof(ExchangePackMessageNonAlignment) << endl << endl;


    InternalEPMa = {
        false,
        TypesMessage::SET,
        Commands::RUN,
        100,
        0,
        0,
        {} // non flexible array
    };

    InternalEPMna = {
        false,
        TypesMessage::SET,
        Commands::STOP,
        100,
        0,
        4,
        {1, 2, 3, 4} // flexible array (last flexible field in class ExchangeProto)
    };

    cout << "------------------------------------------------" << endl;
    cout << " EPMa - alignment structure,EMPna - nonalignment" << endl;
    cout << "------------------------------------------------" << endl;

    cout << "class field \"InternalEPMa\" (with dynamic array member) (static init) " << endl;
    PrintEPM(InternalEPMa);
    cout << "real sizeof(InternalEPMa.payload), init with {} =" << sizeof(InternalEPMa.payload) << ", that pointer!" << endl;

    cout << "------------------------------------------------" << endl;

    cout << "class field \"InternalEPMna\" (with flexible array member) (static init) " << endl;
    PrintEPM(InternalEPMna);

    cout << "------------------------------------------------" << endl;


    cout << "class field \"InternalEPMa\", after dynamic setting of field \"payload\", !random content!:" << endl;
    InternalEPMa.payloadLength = 6;
    InternalEPMa.payload = new uint8_t[InternalEPMa.payloadLength];
    PrintEPM(InternalEPMa);
    cout << "pointer sizeof(InternalEPMa.payload), as \'new uint8_t[6]\' =" << sizeof(InternalEPMa.payload) << endl;

    cout << "------------------------------------------------" << endl;
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

template<typename T> void ExchangeProto::PrintEPM(const T &epm)
{
    cout << "  direction: " << epm.direction << endl;
    cout << "  TypeMsg: " << epm.typeOfMessage << endl;
    cout << "  Command: " << epm.command << endl;
    cout << "  idOfSensor: " << (int)epm.idOfSensor << endl;
    cout << "  simplecrc: " << epm.simplecrc << endl;
    cout << "  payloadLength: " << (int)epm.payloadLength << endl;

    //invalid application of ‘sizeof’ to incomplete type ‘const uint8_t []
    if (epm.payloadLength > 0) {
        cout << "  payload: length=" << (int)epm.payloadLength << ", sizeof= _" << this->mySizeof(epm);
        cout << "_; single element size = _" << this->mySizeof(epm) / (int)epm.payloadLength;
        cout << "_; field typeid = " << typeid(epm.payload).name();
        cout << "; element typeid = " << typeid(epm.payload[0]).name() << endl;
        cout << "    correct? sizeof(payloadLength *sizeof(uint8_t))=" << epm.payloadLength *sizeof(uint8_t) << endl;
        for (int i = 0; i < epm.payloadLength; ++i) {
            cout << "  payload: [" << i << "], sizeof=" << sizeof(epm.payload[i]) << " value=" << (int)epm.payload[i] << endl;
        }
    } else {
        cout << "  payload: empty, sizeof=" << this->mySizeof(epm) << endl;
    }

}

template<typename T> int ExchangeProto::mySizeof(const T &epm)
{
    return epm.payloadLength * sizeof(typeid(epm.payload));
}
