#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QMainWindow>
#include <qcustomplot.h>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void portnameChange(void);
    void baudrateChange(void);
    void portsRefresh(void);
    void openPort(void);
    void closePort(void);
    void serialReadData(void);
    void serialWriteData(void);
    void setAutolog(void);
    void disAutolog(void);
    void selectLogFile(void);
private:
    void updateData(QString data);
private:
    Ui::MainWindow *ui;
private:
    QList<QAction*> _actions;
    QList<QAction*> _baudActions;
    QList<QAction*> _logActions;
    QAction *_currentNameAction;
private:
    QVector<double> _timeVect;
    QVector<double> _aXVect;
    QVector<double> _aYVect;
    QVector<double> _aZVect;
    QVector<double> _gXVect;
    QVector<double> _gYVect;
    QVector<double> _gZVect;
    QVector<double> _mXVect;
    QVector<double> _mYVect;
    QVector<double> _mZVect;
    QVector<double> _pVect;
    QVector<double> _hVect;
    QVector<double> _tVect;
    QVector<double> _idVect;
    double _temptime;
    int _sumIntervals;
    int _countIntervals;
private:
    QString _portName;
    QString _logfileName;
    QSerialPortInfo *_portInfo;
    QSerialPort *_serialPort;
    uint32_t _baud;
    bool _serialOpened;
private:
    QFile *_logFile;
    QTextStream *_logFileStream;
    bool _useLogging;
};
#endif // MAINWINDOW_H
