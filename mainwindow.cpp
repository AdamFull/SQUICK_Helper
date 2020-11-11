#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QString>
#include <QFileDialog>

/***********************************************************************/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    UpdateSerialList();

    if (!connect(&SerialPort, &QSerialPort::readyRead, this, &MainWindow::Listener)){
        Q_ASSERT(0&&"Serial port signal connect error.");
        ui->t_cmd->append("Signal-slot connection error. Update your app.");
    }

    ui->pb_progress->setVisible(false);

    /*customGraph = new CustomGraph();
     ui->chart_layout->addWidget(customGraph->getGraphWidget());

     customGraph->add_value(5);
     customGraph->add_value(1);
     customGraph->add_value(6);
     customGraph->add_value(2);
     customGraph->update();*/
}

/***********************************************************************/
MainWindow::~MainWindow(){
    if (!uartWorker)
        delete uartWorker;
    delete ui;
}

/***********************************************************************/
void MainWindow::on_b_com_update_clicked(){
    /*Update com list*/
    UpdateSerialList();
}

/***********************************************************************/
void MainWindow::on_b_com_connect_clicked(){
    uartWorker = new UARTWorker();

    if(!connect(uartWorker, &UARTWorker::bytesToReceiveSignal, ui->pb_progress, &QProgressBar::setMaximum)){
        Q_ASSERT(0&&"Progress bar signal connect error.");
        ui->t_cmd->append("Signal-slot connection error. Progress bar will not work.");
    }

    if(!connect(uartWorker, &UARTWorker::bytesReceivedSignal, ui->pb_progress, &QProgressBar::setValue)){
        Q_ASSERT(0&&"Progress bar signal connect error.");
        ui->t_cmd->append("Signal-slot connection error. Progress bar will not work.");
    }

    if(!connect(uartWorker, &UARTWorker::fileReceivingDone, this, &MainWindow::fileReceived)){
        Q_ASSERT(0&&"Progress bar signal connect error.");
        ui->t_cmd->append("Signal-slot connection error. Progress bar will not work.");
    }

    /*Connect to selected com*/
    SerialPort.setPortName(ui->cb_com_port->currentText());\
    SerialPort.setBaudRate(ui->cb_baudrate->currentText().toInt());
    SerialPort.setDataBits(QSerialPort::Data8);
    SerialPort.setParity(QSerialPort::NoParity);
    SerialPort.setStopBits(QSerialPort::OneStop);
    SerialPort.setFlowControl(QSerialPort::NoFlowControl);
    if(SerialPort.open(QIODevice::ReadWrite)){
        ui->b_com_update->setEnabled(false);
        ui->b_com_connect->setEnabled(false);
        ui->b_com_disconnect->setEnabled(true);
        ui->cb_baudrate->setEnabled(false);
        ui->cb_com_port->setEnabled(false);
        ui->actionDownload_geiger_data->setEnabled(true);
        ui->gb_log->setEnabled(true);
        ui->t_cmd->append("STATUS: Connected succesfully.");
    }else{
        ui->t_cmd->append("STATUS: Failed to connect.");
    }
}

/***********************************************************************/
void MainWindow::on_b_com_disconnect_clicked(){
    if (!uartWorker)
        delete uartWorker;

    /*Disconnect from current com*/
    SerialPort.close();
    ui->b_com_update->setEnabled(true);
    ui->b_com_connect->setEnabled(true);
    ui->b_com_disconnect->setEnabled(false);
    ui->cb_baudrate->setEnabled(true);
    ui->cb_com_port->setEnabled(true);
    ui->actionDownload_geiger_data->setEnabled(false);
    ui->gb_log->setEnabled(false);
    ui->t_cmd->append("STATUS: Disconnected.");
}

/***********************************************************************/
void MainWindow::on_b_send_cmd_clicked(){
    Sender(ui->le_cmd->text().toUtf8());
    ui->le_cmd->clear();
}

/***********************************************************************/
void MainWindow::UpdateSerialList(){
    /*Read serial ports, that you can use now*/
    ui->cb_com_port->clear();
    int i = 0;
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        ui->cb_com_port->addItem(info.portName());
        ui->cb_com_port->setItemData(i, info.description(), Qt::ToolTipRole);
        i++;
    }
}

/***********************************************************************/
void MainWindow::Sender(const QByteArray data){
    /*Send data to device*/
    ui->t_cmd->append(QString("Sended: %1.").arg(QString(data.trimmed())));
    SerialPort.write(data);
}

/***********************************************************************/
void MainWindow::Listener(){
    /*Receive data from device*/
    SerialPort.flush();
    const QByteArray data = SerialPort.readAll();
    uartWorker->setMessage(data);
    ui->t_cmd->append(QString("Received: %1.").arg(QString(data.trimmed())));
}

/***********************************************************************/
void MainWindow::on_le_cmd_returnPressed(){
    on_b_send_cmd_clicked();
}

/***********************************************************************/
void MainWindow::fileReceived(){
    ui->pb_progress->setValue(0);
    ui->pb_progress->setVisible(false);
    ui->t_cmd->append("File receiving done.");
}

/***********************************************************************/
void MainWindow::on_actionDownload_geiger_data_triggered(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Table files (*.csv);;Text files (*.txt)"));
    if(!fileName.isEmpty()){
        ui->pb_progress->setVisible(true);
        uartWorker->openFile(fileName);
        Sender(QByteArray("rdlog"));
    }else{
        ui->t_cmd->append("File was not saved.");
    }
}

/***********************************************************************/
void MainWindow::on_actionUpdate_firmware_triggered(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Firmware (*.hex)"));
    if(!fileName.isEmpty()){
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            ui->t_cmd->append(QString("Error while opening: %1").arg(fileName));
            return;
        }
        file.close();
    }else{
        ui->t_cmd->append("File was not selected.");
    }
}
