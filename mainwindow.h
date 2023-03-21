#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QtCore/qmath.h>

#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "port.h"
#include <cmath>

#include "qcustomplot.h"

#include <QFile>
#include <QFileDialog>
#include <QTextStream>

#include <QIODevice>
#include <QMessageBox>

#include <QMessageBox>
#include <QDesktopWidget>
#include <QScreen>
#include <QMetaEnum>
#include <errno.h>

#include <QString>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h>

#include <QApplication>
#include <QPushButton>

# define M_PI           3.14159265358979323846  /* pi */

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void    parseMath(QString data);
    void    dots(double r);
    void    dark_theme();
    void    light_theme();

    QJsonObject recordObject;

    QJsonArray numbersArray_y;
    QJsonArray numbersArray_angle;


private:
    Ui::MainWindow *ui;

    QTimer *timer_6;
    QTimer *timer_7;

    QThread *thread_New;

    QString data_in;
    QByteArray pw;

    bool    start_dots;
    bool    flag_start;
    double  key;
    int     save;

    double  y_1[2];

    double  angle[2];

    double  k;
    double  S;
    double  B; //Значение из UI
    double  arg1_B;
    double  Td;

    double  r_0;
    double  r_1;
    double  delta;

    int     i_1;
    int     i;

    double  u[2];
    double  tmp_r[2];

    bool theme;

    const QString TEXT_FILE_FORMAT = "Json Files (*.json)";
    const QString ALL_FILE_FORMAT = "All Files (*.*)";

signals:
    void savesettings(QString name, int baudrate, int DataBits, int Parity, int StopBits, int FlowControl);
    void writeData(QByteArray data);

private slots:

    void    checkCustomBaudRatePolicy(int idx);
    void    on_Btn_Serch_clicked();
    void    on_BtnSave_clicked();
    void    Print(QString data);
    void    on_pushButton_clicked();
    void    start();
    void    stop();
//    void    realtimeDataSlot();
    void    realtimeDataSlot_2();
    void on_doubleSpinBox_228_valueChanged(double arg1);
};
#endif // MAINWINDOW_H
