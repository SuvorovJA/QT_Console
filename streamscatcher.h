#ifndef STREAMSCATCHER_H
#define STREAMSCATCHER_H

#include <sstream>
#include <QString>
#include <QObject>

class StreamsCatcher : public QObject
{
    Q_OBJECT

public:
    StreamsCatcher();

private:
    // перенаправление cout, приватные поля класса, буфера стрингстрим
//  std::stringstream ssin;
    std::stringstream ssout;
    std::stringstream sserr;
    std::stringstream sslog;
    //
    QString mygetline(std::stringstream &);
    void oneLinefromStreamToBrowser(std::stringstream &);
    void streamBufferToBrowser(std::stringstream &, Qt::GlobalColor);

signals:
    void insertPlainTextSignal(const QString &);
    void setTextColorSigal(const Qt::GlobalColor);
    void timerStopSignal();
    void timerStartSignal();

public slots:
    void catchStream(); // by timer driven

};


#endif // STREAMSCATCHER_H
