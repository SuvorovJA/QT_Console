#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include"QTimer"

#include"streamscatcher.h"
#include"dialogboxss.h"
#include"mymain.h"

namespace Ui {
class MainWindow;
}

/*
 * QT_Console
 * by SuvorovJA
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

//    friend class StreamsCatcher;
//    friend class DialogBoxSS;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static DialogBoxSS *dbholder;

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    int timerDelay = 500;
    StreamsCatcher *scholder;

signals:
    void returnDialogBoxSignal(const std::string &);
    void returnDialogBoxCancelledSignal();

public slots:
    void insertPlainTextSlot(const QString &);
    void setTextColorSlot(const Qt::GlobalColor &);
    void timerStopSlot();
    void timerStartSlot();
    void inputDialogBoxSlot(const std::string &);

protected slots:
    void startButtonClicked();
    void cBoxFontSizesSelected();
    void fontComboBoxSelected();

};

#endif // MAINWINDOW_H
