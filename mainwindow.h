#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "uartworker.h"
#include "customgraph.h"

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
    void on_b_com_update_clicked();
    void on_b_com_connect_clicked();
    void on_b_com_disconnect_clicked();
    void on_b_send_cmd_clicked();

    void on_le_cmd_returnPressed();

    void fileReceived();

    void on_actionDownload_geiger_data_triggered();

    void on_actionUpdate_firmware_triggered();

private:
    void UpdateSerialList();
    void Sender(const QByteArray);
    void Listener();
    QSerialPort SerialPort;
    Ui::MainWindow *ui;
    UARTWorker *uartWorker;
    CustomGraph *customGraph;
};
#endif // MAINWINDOW_H
