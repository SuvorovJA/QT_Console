/*
 * QT_Console
 * by SuvorovJA
 *
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mymain.h"
#include "QTimer"
#include <string>
#include <sstream>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // таймер для мониторинга буферов перехвата cout cin
    // разделения таймеров сделано для ловли бага (см. readme)
    timerCout = new QTimer(this);
    timerCerr = new QTimer(this); // clog путь будет здесь-же
    connect(timerCout, SIGNAL(timeout()), this, SLOT(catchStreamCout()));
    connect(timerCerr, SIGNAL(timeout()), this, SLOT(catchStreamCerr()));
    timerCout->start(timerDelay);
    timerCerr->start(timerDelay);

    // соединения слотов и сигналов QT
    connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(startButtonClicked()));
    connect(ui->cBoxFontSizes, SIGNAL(currentIndexChanged(int)), this, SLOT(cBoxFontSizesSelected()));
    connect(ui->fontComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(fontComboBoxSelected()));

    // заполнение размера шрифтов в комбобокс
    ui->cBoxFontSizes->addItems({"7", "9", "11", "14", "16", "18"});
    ui->cBoxFontSizes->setCurrentIndex(3); // 14й шрифт выбрать
    // в списке шрифтов показать только моноширинные
    ui->fontComboBox->setFontFilters(QFontComboBox::MonospacedFonts);
    // цвет фона и шрифта для "консоли"
    ui->textBrowser->setTextColor(Qt::white);
    ui->textBrowser->setTextBackgroundColor(Qt::black);
    ui->textBrowser->setStyleSheet("background-color: black;");

    // перехват потоков cin cout cerr clog
//    std::cin.rdbuf(ssin.rdbuf()); //redirect std::cin
    std::cout.rdbuf(ssout.rdbuf()); //redirect std::cout
    std::cerr.rdbuf(sserr.rdbuf()); //redirect std::cerr
    std::clog.rdbuf(sslog.rdbuf()); //redirect std::clog

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startButtonClicked()
{

    //выключить стартовую кнопку
    ui->startButton->setDisabled(true);
    //очистить поле текста и статусбар
    ui->textBrowser->clear();
    ui->statusBar->clearMessage();
    //
    //строку параметров командной строки переделать к виду int argc. char* argv[]
    QString commandline = ui->commandLineEdit->text();
    // разделение по словам с пробелом-разделителем. TODO проверить на несколько пробелов подряд
    int wordCounter = 0;//количество параметров
    QStringList argumensAsQstrList;//параметры списком строк
    if (!commandline.isEmpty()) { // если параметры есть, то разделим их на отдельные слова
        commandline = commandline.simplified(); //убрать лишние пробелы
        argumensAsQstrList = commandline.split(' ');
        wordCounter = argumensAsQstrList.size(); // счетчик слов в строке аргументов
    }
    char **argumens = new char *[wordCounter]; // объявим массив указателей на строки, если 0 - нестрашно?
    // итерации по параметром командной строки, ЭТО конвертация QStringList в *char[]
    // TODO выпилить в функцию ?
    int idx = 0;
    if (!commandline.isEmpty()) { //конвертируем только если строка параметров не пустая
        for (const auto &oneArg : argumensAsQstrList) {
            //для каждой строки выделить память
            argumens[idx] = new char[oneArg.size() + 1];
            //скопировать строку
            strcpy(argumens[idx], (char *)(oneArg.toStdString().c_str()));
            idx++;
        }
    }
    // тут храним код возврата из mymain(), как и обычный main() оно возвращает число
    int mymainReturn = INT_MIN;
    mymainReturn = mymain(wordCounter, argumens); // ВЫЗОВ mymain()
    ui->statusBar->showMessage("Return code: " + QString::number(mymainReturn));

    // из под отдельных строк забирать память ?
    for (int idx = 0; idx < wordCounter; idx++) {
        delete argumens[idx];
    }
    // забрать память из под массива
    delete[] argumens;
}

void MainWindow::cBoxFontSizesSelected()
{
    int fontsize =  ui->cBoxFontSizes->currentText().toInt();
    ui->textBrowser->setFontPointSize(fontsize);
    ui->textBrowser->update();
}

void MainWindow::fontComboBoxSelected()
{
    QFont selection = ui->fontComboBox->currentFont();
    ui->textBrowser->setFont(selection);
    ui->textBrowser->update();
}

void MainWindow::catchStreamCout() // этот метод запускается по таймеру
{
    timerCout->stop(); // выключить таймер пока мы тут
    bool catched     = false; // индикатор необходимости обновить браузер
    // пустой или нет?
    // "ssout.rdbuf()->in_avail() > 0" - способ обнаружить символы в потоке
    (ssout.rdbuf()->in_avail() > 0) ? catched = true : catched = false;
    while (ssout.rdbuf()->in_avail() > 0) {
        ui->textBrowser->insertPlainText(mygetline(ssout)); //докинуть в текстбраузер
    }
    if (catched) {
        ui->textBrowser->update(); // обновить браузер
    }
    timerCout->start(timerDelay); // включить таймер назад
}

void MainWindow::catchStreamCerr() // этот метод запускается по таймеру
{
    timerCerr->stop();
    bool catched_err     = false; // индикатор необходимости обновить браузер
    bool catched_log     = false; // индикатор необходимости обновить браузер
    // пустой или нет?
    (sserr.rdbuf()->in_avail() > 0) ? catched_err = true : catched_err = false;
    if (catched_err) ui->textBrowser->setTextColor(Qt::red);
    while (sserr.rdbuf()->in_avail() > 0) {
        ui->textBrowser->insertPlainText(mygetline(sserr)); //докинуть в текстбраузер
    }
    (sslog.rdbuf()->in_avail() > 0) ? catched_log = true : catched_log = false;
    if (catched_log) ui->textBrowser->setTextColor(Qt::green);
    while (sslog.rdbuf()->in_avail() > 0) {
        ui->textBrowser->insertPlainText(mygetline(sslog)); //докинуть в текстбраузер
    }
    if (catched_err || catched_log) {
        ui->textBrowser->setTextColor(Qt::white);
        ui->textBrowser->update(); // обновить браузер
    }
    timerCerr->start(timerDelay);
}

QString MainWindow::mygetline(std::stringstream &s)
{
    std::string line; // стандартная строка для getline
    getline(s, line); // выдернуть одну строку
    line += '\n'; // добавить конец строки, а то он теряется в гетлайне
    return QString::fromStdString(line); // переделать в qt-формат
}
