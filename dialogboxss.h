#ifndef DIALOGBOXSS_H
#define DIALOGBOXSS_H

#include <string>
#include <QObject>
#include <QMutex>
#include <QWaitCondition>

class DialogBoxSS : public QObject
{

    Q_OBJECT

public:
    DialogBoxSS();
    std::string getString(const std::string &); // use in mymain for get text value
private:
    std::string returnDialogBoxValue;
    bool returnDialogBoxCancelled;
    QMutex mutex;
    QWaitCondition wait;
signals:
    void inputDialogBoxSignal(const std::string &);

public slots:
    void returnDialogBoxSlot(const std::string &);
    void returnDialogBoxCleanSlot();
    void returnDialogBoxCancelledSlot();
};

#endif // DIALOGBOXSS_H
