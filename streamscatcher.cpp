#include "streamscatcher.h"
#include <iostream>
#include <string>
#include <sstream>

StreamsCatcher::StreamsCatcher()
{
    // перехват потоков cin cout cerr clog
//  std::cin.rdbuf(ssin.rdbuf()); //redirect std::cin
    std::cout.rdbuf(ssout.rdbuf()); //redirect std::cout
    std::cerr.rdbuf(sserr.rdbuf()); //redirect std::cerr
    std::clog.rdbuf(sslog.rdbuf()); //redirect std::clog
}

void StreamsCatcher::catchStream() // этот метод запускается по таймеру
{
    emit StreamsCatcher::timerStopSignal(); // выключить таймер пока мы тут
    streamBufferToBrowser(ssout, Qt::white);
    streamBufferToBrowser(sserr, Qt::red);
    streamBufferToBrowser(sslog, Qt::green);
    emit StreamsCatcher::timerStartSignal(); // включить таймер назад
}

QString StreamsCatcher::mygetline(std::stringstream &s)
{
    std::string line; // стандартная строка для getline
    std::getline(s, line); // выдернуть одну строку
//    line += '\n'; // добавить конец строки, а то он теряется в гетлайне (эта строка багует выборку потока)
    return QString::fromStdString(line); // переделать в qt-формат
}

void StreamsCatcher::oneLinefromStreamToBrowser(std::stringstream &s)
{
    QString line = mygetline(s);
    line.push_back("\n");
    emit insertPlainTextSignal(line);
}

void StreamsCatcher::streamBufferToBrowser(std::stringstream &s, Qt::GlobalColor color)
{
    bool catched     = false;
    (s.rdbuf()->in_avail() > 0) ? catched = true : catched = false;
    if (catched && color != Qt::white) emit setTextColorSigal(color);
    while (s.rdbuf()->in_avail() > 0) {
        oneLinefromStreamToBrowser(s);
    }
    if (catched && color != Qt::white) emit setTextColorSigal(Qt::white);
}

