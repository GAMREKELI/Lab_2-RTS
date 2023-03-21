#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    theme = true;
    flag_start = true;

    QPalette darkPalette;

    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    qApp->setPalette(darkPalette);

    start_dots = false;

    y_1[0] = 0.0;
    y_1[1] = 0.0;

    k = 100.0;
    S = 1.0;
    B = 400.0;
    Td = 0.01;

    arg1_B = B;

    r_0 = 0.0;
    r_1 = 0.0;
    delta = 0.0;

    angle[0] = 0.0;
    angle[1] = 0.0;

    i_1 = 0;
    i = 1;

    u[0] = 0.0;
    u[0] = 0.0;

    save = 0;
    tmp_r[0] = 0.0;
    tmp_r[1] = 0.0;

    connect(ui->BaudRateBox, SIGNAL(currentIndexChanged(int)) ,this, SLOT(checkCustomBaudRatePolicy(int)));
    ui->BaudRateBox->addItem(QLatin1String("9600"), QSerialPort::Baud9600);
    ui->BaudRateBox->addItem(QLatin1String("19200"), QSerialPort::Baud19200);
    ui->BaudRateBox->addItem(QLatin1String("38400"), QSerialPort::Baud38400);
    ui->BaudRateBox->addItem(QLatin1String("115200"), QSerialPort::Baud115200);
    ui->BaudRateBox->addItem(QLatin1String("Custom"));

    thread_New = new QThread;//Создаем поток для порта платы
    Port *PortNew = new Port();//Создаем обьект по классу
    PortNew->moveToThread(thread_New);//помешаем класс  в поток
    PortNew->thisPort.moveToThread(thread_New);//Помещаем сам порт в поток


    key = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;


    connect(PortNew, SIGNAL(error_(QString)), this, SLOT(Print(QString)));//Лог ошибок
    connect(thread_New, SIGNAL(started()), PortNew, SLOT(process_Port()));//Переназначения метода run

    connect(PortNew, SIGNAL(finished_Port()), thread_New, SLOT(quit()));//Переназначение метода выход
    connect(thread_New, SIGNAL(finished()), PortNew, SLOT(deleteLater()));//Удалить к чертям поток
    connect(PortNew, SIGNAL(finished_Port()), thread_New, SLOT(deleteLater()));//Удалить к чертям поток
    connect(this,SIGNAL(savesettings(QString,int,int,int,int,int)),PortNew,SLOT(Write_Settings_Port(QString,int)));//Слот - ввод настроек!
    connect(ui->BtnConnect, SIGNAL(clicked()),PortNew,SLOT(ConnectPort()));//по нажатию кнопки подключить порт

    connect(ui->BtnConnect, SIGNAL(clicked()),this,SLOT(start()));
    connect(ui->BtnDisconect, SIGNAL(clicked()),this,SLOT(stop()));

    connect(ui->BtnDisconect, SIGNAL(clicked()),PortNew,SLOT(DisconnectPort()));//по нажатию кнопки отключить порт
    connect(PortNew, SIGNAL(outPort(QString)), this, SLOT(Print(QString)));//вывод в текстовое поле считанных данных

    connect(this,SIGNAL(writeData(QByteArray)),PortNew,SLOT(WriteToPort(QByteArray)));//отправка в порт данных
    thread_New->start();
}

void MainWindow::start()
{
    ui->BtnConnect->setEnabled(false);
    ui->BtnDisconect->setEnabled(true);
    if (flag_start == true)
    {
        usleep(100000);
        data_in = "\r";
        writeData(data_in.toLocal8Bit());
        usleep(50000);
        writeData(data_in.toLocal8Bit());
        sleep(4);
        pw = "les\r";
        writeData(pw);
        flag_start = false;
    }


    key = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
    timer_7 = new QTimer();
    ui->widget_7->clearGraphs();

    ui->widget_7->addGraph(); // blue line
    ui->widget_7->graph(0)->setPen(QPen(QColor(40, 110, 255)));

    QSharedPointer<QCPAxisTickerTime> timeTicker_7(new QCPAxisTickerTime);
    timeTicker_7->setTimeFormat("%h:%m:%s");
    ui->widget_7->xAxis->setTicker(0);
    ui->widget_7->axisRect()->setupFullAxesBox();
    ui->widget_7->yAxis->setRange(-90, 90);

    ui->widget_7->yAxis->setLabel("Angle");

    connect(ui->widget_7->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget_7->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->widget_7->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget_7->yAxis2, SLOT(setRange(QCPRange)));

    connect(timer_7, SIGNAL(timeout()), this, SLOT(realtimeDataSlot_2()));
//    ui->widget_7->setInteraction(QCP::iRangeZoom, true);
    ui->widget_7->setInteraction(QCP::iRangeDrag, true);
    ui->widget_7->replot();
    timer_7->start(0);
}


void MainWindow::stop()
{
    ui->BtnConnect->setEnabled(true);
    ui->BtnDisconect->setEnabled(false);

//    timer_6->stop();
    timer_7->stop();

//    recordObject.insert("Y1",numbersArray_y);
    recordObject.insert("ANGLE",numbersArray_angle);

    QJsonDocument doc(recordObject);
    QString jsonString = doc.toJson(QJsonDocument::Indented);

    QFile file;

    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"), "", TEXT_FILE_FORMAT + ";;" + ALL_FILE_FORMAT);
    if (!filePath.isEmpty()) {
        QTextStream Qcout(stdout);
        Qcout<<filePath;
        QString filePath_tmp;
        ++save;

        QString save_tmp = QString::number(save);

        filePath_tmp = filePath + "_" + save_tmp + ".json";
        file.setFileName(filePath_tmp);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream stream( &file );
        stream << jsonString;

        file.close();

        QString fileNameGraf_2(filePath + "ANGLE" + "_" + save_tmp + ".bmp");
        QFile file_2(fileNameGraf_2);
        if (!file_2.open(QIODevice::WriteOnly))
        {
            qDebug() << file_2.errorString();
        }
        else
        {
            ui->widget_7->saveBmp(fileNameGraf_2, 800, 800);
        }
    }

    u[0] = 0.0;
    u[1] = 0.0;

    y_1[0] = 0.0;
    y_1[1] = 0.0;

    angle[0] = 0.0;
    angle[1] = 0.0;

    i_1 = 0;
    i = 1;

    r_0 = 0.0;
    r_1 = 0.0;
    B = arg1_B;
}

void MainWindow::dark_theme()
{
    QPalette darkPalette;

    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    qApp->setPalette(darkPalette);
    theme = true;
}

void MainWindow::realtimeDataSlot_2()
{
    if (start_dots == true)
    {
        ui->widget_7->graph(0)->addData(i_1, angle[1]);
        ui->widget_7->xAxis->setRangeUpper(i_1 + 1);
        ui->widget_7->replot();
   }
}

void MainWindow::light_theme()
{
    qApp->setPalette(style()->standardPalette());
    theme = false;
}

void MainWindow::on_Btn_Serch_clicked()
{
    ui->PortNameBox->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
        ui->PortNameBox->addItem(info.portName());
        }
}

void MainWindow::checkCustomBaudRatePolicy(int idx)
{
    bool isCustomBaudRate = !ui->BaudRateBox->itemData(idx).isValid();
    ui->BaudRateBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate) {
        ui->BaudRateBox->clearEditText();
    }
}

void    MainWindow::dots(double delta)
{
    ++ i_1;

    if (i_1 == 1)
    {
        angle[i_1 - 1] = (asin(delta * 1000.0 / B) * 180.0 / M_PI);
        u[i_1 - 1] = angle[i_1 - 1];
    }
    if (i_1 >= 2)
    {

        // Реализация расчёта
        angle[i_1 - i] = (asin(delta * 1000.0 / B) * 180.0 / M_PI);
        if (qIsNaN(angle[i_1 - i])) {
            angle[i_1 - i] = 0.0;
        }
        else
        {
            if (fabs(angle[i_1 - i] - angle[i_1 - i - 1]) >= 30.0)
                angle[i_1 - i] = angle[i_1 - i - 1];
            if (!angle[i_1 - i])
                angle[i_1 - i] = 0.0;

            u[i_1 - i] = angle[i_1 - i];

            y_1[i_1 - i] = y_1[i_1 - i - 1] * (k * S * Td - 1 ) + Td * S * u[i_1 - i - 1];

            u[i_1 - i - 1] = u[i_1 - i];

            y_1[i_1 - i - 1] = y_1[i_1 - i];

            angle[i_1 - i - 1] = angle[i_1 - i];

            numbersArray_y.push_back(y_1[i_1 - i]);
            numbersArray_angle.push_back(angle[i_1 - i]);
        }
        ++ i;
    }
}

void MainWindow::parseMath(QString data)
{
    QRegExp rx("\\=([0-9\\.]+)"); // регулярное выражение для поиска чисел после знака равно
    int     pos = 0;
    int     k = 0;
    bool    flag = true;
    while ((pos = rx.indexIn(data, pos)) != -1)
    {
        QString num = rx.cap(1); // извлекаем число из первой группы
        tmp_r[k] = num.toDouble();
        pos += rx.matchedLength(); // переходим к следующему вхождению
        ++ k;
    }

    if (flag)
    {

        r_0 = tmp_r[0];
        r_1 = tmp_r[1];

        delta = r_0 - r_1;
        start_dots = false;
        dots(delta);
        delta = 0.0;
        start_dots = true;
    }

}

void MainWindow::Print(QString data)
{
    parseMath(data);
    ui->consol->textCursor().insertText(data+'\r'); // Вывод текста в консоль
    ui->consol->moveCursor(QTextCursor::End);//Scroll
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_BtnSave_clicked()
{

    savesettings(ui->PortNameBox->currentText(), ui->BaudRateBox->currentText().toInt(), 8,
             0, 2, 0);
    ui->BtnConnect->setEnabled(true);

}


void MainWindow::on_pushButton_clicked()
{
    if (theme == true)
        light_theme();
    else
        dark_theme();
}


void MainWindow::on_doubleSpinBox_228_valueChanged(double arg1)
{
    B = arg1;
    arg1_B = arg1;
}

