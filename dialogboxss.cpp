#include "dialogboxss.h"
#include <QCoreApplication>

DialogBoxSS::DialogBoxSS()
{
//  returnDialogBoxValue = "";
    returnDialogBoxCleanSlot();
    connect(this, SIGNAL(inputDialogBoxSignal(const std::string &)), this, SLOT(returnDialogBoxCleanSlot()));
}

void DialogBoxSS::returnDialogBoxSlot(const std::string &line)
{
    returnDialogBoxValue = line;
//    wait.wakeAll();
}

void DialogBoxSS::returnDialogBoxCleanSlot()
{
    returnDialogBoxValue = "";
    returnDialogBoxCancelled = false;
}

void DialogBoxSS::returnDialogBoxCancelledSlot()
{
    returnDialogBoxCancelled = true;
//    wait.wakeAll();
}

std::string DialogBoxSS::getString(const std::string &message)
{
    emit inputDialogBoxSignal(message); // automatic cleaned return value by signal/slot
    while (returnDialogBoxValue.empty() && !returnDialogBoxCancelled) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100); // hungup for wait here
    }
    return returnDialogBoxValue;
}
