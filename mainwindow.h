#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<sstream>
#include"QTimer"

namespace Ui {
class MainWindow;
}

/*
 * QT_Console
 * by SuvorovJA
 *
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private:
    Ui::MainWindow *ui;
    // перенаправление cout, приватные поля класса, буфера стрингстрим
//    std::stringstream ssin;
    std::stringstream ssout;
    std::stringstream sserr;
    std::stringstream sslog;
    // прочий приватный staff
    QString mygetline(std::stringstream &);
    void oneLinefromStreamToBrowser(std::stringstream &);
    void streamBufferToBrowser(std::stringstream &, Qt::GlobalColor);
    QTimer *timerCout;
    QTimer *timerCerr;
    int timerDelay = 500;



protected slots:
    void startButtonClicked();
    void cBoxFontSizesSelected();
    void fontComboBoxSelected();
    void catchStreamCout();
    void catchStreamCerr();

};

#endif // MAINWINDOW_H
