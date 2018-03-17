/*
 * QT_Console
 * by SuvorovJA
 *
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mymain.h"

#include "streamscatcher.h"
#include "dialogboxss.h"

#include "QTimer"
#include "QInputDialog"

//инициализация статического поля класса
DialogBoxSS *MainWindow::dbholder = new DialogBoxSS();

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // объект StreamsCatcher
    scholder = new StreamsCatcher();

    // таймер для мониторинга буферов перехвата cout cin
    // разделения таймеров сделано для ловли бага (см. readme)
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), scholder, SLOT(catchStream()));
    timer->start(timerDelay);

    // соединения слотов и сигналов QT
    connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(startButtonClicked()));
    connect(ui->cBoxFontSizes, SIGNAL(currentIndexChanged(int)), this, SLOT(cBoxFontSizesSelected()));
    connect(ui->fontComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(fontComboBoxSelected()));

    // соединения слотов и сигналов DialogBoxSS
    connect(this, SIGNAL(returnDialogBoxCancelledSignal()), this->dbholder, SLOT(returnDialogBoxCancelledSlot()));
    connect(this, SIGNAL(returnDialogBoxSignal(std::string)), dbholder, SLOT(returnDialogBoxSlot(std::string)));
    connect(dbholder, SIGNAL(inputDialogBoxSignal(const std::string &)), this, SLOT(inputDialogBoxSlot(const std::string &)));
    // соединения слотов и сигналов StreamsCatcher
    connect(scholder, SIGNAL(timerStartSignal()), this, SLOT(timerStartSlot()));
    connect(scholder, SIGNAL(timerStopSignal()), this, SLOT(timerStopSlot()));
    connect(scholder, SIGNAL(insertPlainTextSignal(QString)), this, SLOT(insertPlainTextSlot(QString)));
    connect(scholder, SIGNAL(setTextColorSigal(Qt::GlobalColor)), this, SLOT(setTextColorSlot(Qt::GlobalColor)));

    // заполнение размера шрифтов в комбобокс
    ui->cBoxFontSizes->addItems({"7", "9", "11", "14", "16", "18"});
    ui->cBoxFontSizes->setCurrentIndex(2); // 14й шрифт выбрать
    // в списке шрифтов показать только моноширинные
    ui->fontComboBox->setFontFilters(QFontComboBox::MonospacedFonts);
    // цвет фона и шрифта для "консоли"
    ui->textBrowser->setTextColor(Qt::white);
    ui->textBrowser->setTextBackgroundColor(Qt::black);
    ui->textBrowser->setStyleSheet("background-color: black;");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startButtonClicked()
{
    //выключить стартовую кнопку
    // ui->startButton->setDisabled(true);
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

// StreamsCatcher Slot/Signals

void MainWindow::insertPlainTextSlot(const QString &line)
{
    ui->textBrowser->insertPlainText(line);
    ui->textBrowser->update();
}

void MainWindow::setTextColorSlot(const Qt::GlobalColor &color)
{
    ui->textBrowser->setTextColor(color);
    ui->textBrowser->update();
}

void MainWindow::timerStopSlot()
{
    timer->stop();
}

void MainWindow::timerStartSlot()
{
    timer->start(timerDelay);
}

void MainWindow::inputDialogBoxSlot(const std::string &message)
{
    QString msg = QString::fromStdString(message);
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"), msg, QLineEdit::Normal, tr(""), &ok);
    if (ok && !text.isEmpty()) emit returnDialogBoxSignal(text.toStdString());
    if (!ok || text.isEmpty()) emit returnDialogBoxCancelledSignal();
    // what about any time go here ?
}
