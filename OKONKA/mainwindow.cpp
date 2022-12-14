#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPen XPen;
    QPen YPen;
    QPen ZPen;
    XPen.setWidth(1);
    XPen.setColor(QColor(255, 0, 0));
    YPen.setWidth(1);
    YPen.setColor(QColor(0, 255, 0));
    ZPen.setWidth(1);
    ZPen.setColor(QColor(0, 0, 255));
    ui->pressPlot->addGraph();
    ui->pressPlot->graph(0)->setLineStyle(QCPGraph::lsImpulse);
    ui->tempPlot->addGraph();
    ui->tempPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->humPlot->addGraph();
    ui->humPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->axelPlot->addGraph();
    ui->axelPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->axelPlot->graph(0)->setPen(XPen);
    ui->axelPlot->addGraph();
    ui->axelPlot->graph(1)->setLineStyle(QCPGraph::lsLine);
    ui->axelPlot->graph(1)->setPen(YPen);
    ui->axelPlot->addGraph();
    ui->axelPlot->graph(2)->setLineStyle(QCPGraph::lsLine);
    ui->axelPlot->graph(2)->setPen(ZPen);
    // удалить позже
    ui->axelPlot->hide();
    ui->gyroPlot->addGraph();
    ui->gyroPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->gyroPlot->graph(0)->setPen(XPen);
    ui->gyroPlot->addGraph();
    ui->gyroPlot->graph(1)->setLineStyle(QCPGraph::lsLine);
    ui->gyroPlot->graph(1)->setPen(YPen);
    ui->gyroPlot->addGraph();
    ui->gyroPlot->graph(2)->setLineStyle(QCPGraph::lsLine);
    ui->gyroPlot->graph(2)->setPen(ZPen);
    // удалить позже
    ui->gyroPlot->hide();
    ui->magPlot->addGraph();
    ui->magPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->magPlot->graph(0)->setPen(XPen);
    ui->magPlot->addGraph();
    ui->magPlot->graph(1)->setLineStyle(QCPGraph::lsLine);
    ui->magPlot->graph(1)->setPen(YPen);
    ui->magPlot->addGraph();
    ui->magPlot->graph(2)->setLineStyle(QCPGraph::lsLine);
    ui->magPlot->graph(2)->setPen(ZPen);
    // удалить позже
    ui->magPlot->hide();
    _portInfo = nullptr;
    _serialPort = nullptr;
    _currentNameAction = nullptr;
    _logFile = nullptr;
    _serialOpened = false;
    _baud = 115200;
    _useLogging = false;
    _temptime = 0;
    portsRefresh();
    connect(ui->actionRefresh, SIGNAL(triggered(bool)), this, SLOT(portsRefresh()));
    connect(ui->openButton, SIGNAL(clicked(bool)), this, SLOT(openPort()));
    connect(ui->closeButton, SIGNAL(clicked(bool)), this, SLOT(closePort()));
    connect(ui->actionEnabled, SIGNAL(triggered(bool)), this, SLOT(setAutolog()));
    connect(ui->actionDisabled, SIGNAL(triggered(bool)), this, SLOT(disAutolog()));
    connect(ui->actionFilename, SIGNAL(triggered(bool)), this, SLOT(selectLogFile()));
    connect(ui->writeEdit, SIGNAL(returnPressed()), this, SLOT(serialWriteData()));
}

MainWindow::~MainWindow()
{
    for (uint8_t i = 0; i < _actions.length(); i++) delete _actions[i];
    delete ui;
}

void MainWindow::updateData(QString data) {
    QStringList dataStrings = data.split("$^");
    dataStrings.removeLast();
    for (uint8_t i = 0; i < dataStrings.length(); i++) {
        if (dataStrings[i].startsWith("$TD")) {
            double tempt = (_timeVect.length() != 0) ? _timeVect.last() : 0;
            QStringList dataList = dataStrings[i].split('|');
            if (_timeVect.length() > 200) _timeVect.removeFirst();
            _timeVect.append(dataList[14].toDouble());
            if (_aXVect.length() > 200) _aXVect.removeFirst();
            _aXVect.append(dataList[1].toDouble());
            if (_aYVect.length() > 200) _aYVect.removeFirst();
            _aYVect.append(dataList[2].toDouble());
            if (_aZVect.length() > 200) _aZVect.removeFirst();
            _aZVect.append(dataList[3].toDouble());
            if (_gXVect.length() > 200) _gXVect.removeFirst();
            _gXVect.append(dataList[4].toDouble());
            if (_gYVect.length() > 200) _gYVect.removeFirst();
            _gYVect.append(dataList[5].toDouble());
            if (_gZVect.length() > 200) _gZVect.removeFirst();
            _gZVect.append(dataList[6].toDouble());
            if (_mXVect.length() > 200) _mXVect.removeFirst();
            _mXVect.append(dataList[7].toDouble());
            if (_mYVect.length() > 200) _mYVect.removeFirst();
            _mYVect.append(dataList[8].toDouble());
            if (_mZVect.length() > 200) _mZVect.removeFirst();
            _mZVect.append(dataList[9].toDouble());
            if (_pVect.length() > 200) _pVect.removeFirst();
            _pVect.append(dataList[10].toDouble());
            ui->altLCD->display(dataList[11].toDouble());
            if (_hVect.length() > 200) _hVect.removeFirst();
            _hVect.append(dataList[12].toDouble());
            if (_tVect.length() > 200) _tVect.removeFirst();
            _tVect.append(dataList[13].toDouble());
            if (_idVect.length() > 200) _idVect.removeFirst();
            _idVect.append(dataList[15].toDouble());
            _sumIntervals += (_timeVect.last() - tempt);
            _countIntervals++;
            ui->timeLCD->display(int(_timeVect.last() / 1000));
            ui->tempLCD->display(_tVect.last());
            ui->idLCD->display(_idVect.last());
            ui->pressLCD->display(_pVect.last() / 100);
            if (_timeVect.last() - _temptime > 1000) {
                ui->freqLCD->display((_timeVect.last() - _temptime) / (_sumIntervals / _countIntervals));
                _temptime = _timeVect.last();
                _sumIntervals = 0;
                _countIntervals = 0;
            }
            ui->pressPlot->graph(0)->setData(_timeVect, _pVect);
            ui->pressPlot->rescaleAxes();
            ui->pressPlot->replot();
            ui->pressPlot->update();
            ui->tempPlot->graph(0)->setData(_timeVect, _tVect);
            ui->tempPlot->rescaleAxes();
            ui->tempPlot->replot();
            ui->tempPlot->update();
            ui->humPlot->graph(0)->setData(_timeVect, _hVect);
            ui->humPlot->rescaleAxes();
            ui->humPlot->replot();
            ui->humPlot->update();
            ui->axelPlot->graph(0)->setData(_timeVect, _aXVect);
            ui->axelPlot->graph(1)->setData(_timeVect, _aYVect);
            ui->axelPlot->graph(2)->setData(_timeVect, _aZVect);
            ui->axelPlot->rescaleAxes();
            ui->axelPlot->replot();
            ui->axelPlot->update();
            ui->gyroPlot->graph(0)->setData(_timeVect, _gXVect);
            ui->gyroPlot->graph(1)->setData(_timeVect, _gYVect);
            ui->gyroPlot->graph(2)->setData(_timeVect, _gZVect);
            ui->gyroPlot->rescaleAxes();
            ui->gyroPlot->replot();
            ui->gyroPlot->update();
            ui->magPlot->graph(0)->setData(_timeVect, _mXVect);
            ui->magPlot->graph(1)->setData(_timeVect, _mYVect);
            ui->magPlot->graph(2)->setData(_timeVect, _mZVect);
            ui->magPlot->rescaleAxes();
            ui->magPlot->replot();
            ui->magPlot->update();
        } else continue;
    }
}

void MainWindow::selectLogFile(void) {
    _logfileName = QFileDialog::getOpenFileName(this, tr("Open logfile"), "", tr("TXT Files (*.txt);; LOG Files (*.log)"));
    if (_logfileName.isNull()) return;
    else {
        _logFile = new QFile(_logfileName);
        if (_logFile->open(QIODevice::ReadWrite)) {
            _logFileStream = new QTextStream(_logFile);
            *_logFileStream << "Log file inited by SRQGS." << Qt::endl;
        }
    }
}

void MainWindow::setAutolog(void) {
    ui->actionDisabled->setChecked(false);
    _useLogging = true;
}

void MainWindow::disAutolog(void) {
    ui->actionEnabled->setChecked(false);
    _useLogging = false;
}

void MainWindow::serialReadData(void) {
    if (_serialOpened) {
        QByteArray byteData = _serialPort->readAll();
        QString stringData = QString(byteData);
        updateData(stringData);
        if (_useLogging) *_logFileStream << stringData << Qt::endl;
        ui->historyEdit->moveCursor(QTextCursor::End);
        ui->historyEdit->insertPlainText(stringData);
        ui->historyEdit->moveCursor(QTextCursor::End);
    }
}

void MainWindow::serialWriteData(void) {
    ui->historyEdit->moveCursor(QTextCursor::End);
    ui->historyEdit->insertPlainText(ui->writeEdit->text());
    ui->historyEdit->moveCursor(QTextCursor::End);
    if (_serialOpened) {
        QByteArray data;
        data.append(ui->writeEdit->text().toUtf8());
        _serialPort->write(data);
    }
}

void MainWindow::openPort() {
    _serialPort = new QSerialPort(this);
    _serialPort->setPortName(_portName);
    _serialPort->setBaudRate((_baud == 115200) ? QSerialPort::Baud115200 : QSerialPort::Baud9600);
    _serialPort->setDataBits(QSerialPort::Data8);
    _serialPort->setParity(QSerialPort::NoParity);
    _serialPort->setStopBits(QSerialPort::OneStop);
    _serialPort->setFlowControl(QSerialPort::NoFlowControl);
    if (_serialPort->open(QIODevice::ReadWrite)) {
        _serialOpened = true;
        connect(_serialPort, SIGNAL(readyRead()), this, SLOT(serialReadData()));
    } else {
        delete _serialPort;
        _serialPort = nullptr;
    }
}

void MainWindow::closePort() {
    if (!_serialOpened) return;
    _serialOpened = false;
    disconnect(_serialPort, SIGNAL(readyRead()), this, SLOT(serialReadData()));
    delete _serialPort;
    _serialPort = nullptr;
}

void MainWindow::portsRefresh(void) {
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        QAction *action = new QAction("/dev/"+serialPortInfo.portName());
        action->setCheckable(true);
        action->setChecked(false);
        _actions.append(action);
        ui->menuName->addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(portnameChange()));
    }
}

void MainWindow::portnameChange(void) {
    for (uint8_t i = 0; i < _actions.length(); i++) {
        if (_actions[i]->isChecked() && (_actions[i] != _currentNameAction)) {
            _currentNameAction = _actions[i];
            _portName = _currentNameAction->text();
            for (uint8_t j = 0; j < _actions.length(); j++) {
                if (_actions[j] != _currentNameAction) _actions[j]->setChecked(false);
            } break;
        }
    }
}

void MainWindow::baudrateChange(void) {
    qDebug() << "act->text()";
}
