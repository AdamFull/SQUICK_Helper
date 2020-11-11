#include "uartworker.h"
#include <QDebug>
#include <QDataStream>

UARTWorker::UARTWorker(QObject *parent) : QObject(parent){
    /*Когда запустим поток, стартуем что будем делать*/
    if (!connect(&workerThread, &QThread::started, this, &UARTWorker::run))
         Q_ASSERT(0&&"signal-slot for thread connect error.");

    /*Ну и что делать когда его завершаем*/
    if (!connect(this, &UARTWorker::finished, &workerThread, &QThread::terminate))
        Q_ASSERT(0&&"signal-slot for thread connect error.");

    this->moveToThread(&workerThread);
    setRunning(true);
    workerThread.start();
}

UARTWorker::~UARTWorker(){
    setRunning(false);
    workerThread.quit();
    while(workerThread.isRunning()){
        workerThread.terminate();
        QThread::sleep(1);
    }
}

bool UARTWorker::running() const{
    return m_running;
}

QByteArray UARTWorker::message() const{
    return m_message;
}

bool UARTWorker::fileopen() const
{
    return m_fileopen;
}

void UARTWorker::run(){
    bool is_first_line = true;
    int bytes_to_receive = 0;
    int received_bytes = 0;
    while(running()){
        if(!m_message.isEmpty()){
            //qDebug() << "Received:" << m_message << " len: " << m_message.length() << "\r\n";
            if(fileopen()){
                if(is_first_line){
                    is_first_line = false;
                    bytes_to_receive = m_message.trimmed().toUInt();
                    emit bytesToReceiveSignal(bytes_to_receive);
                    qDebug() << "Bytes to receive: " << bytes_to_receive << "\r\n";
                    //Сделать передачу числа байт которых нужно принять в прогресбар
                    m_message.clear();
                    continue;
                }
                int j = 0;
                while ((j = m_message.indexOf("done", j)) != -1) {
                    qDebug() << "Done, file saved." << j;
                    ++j;
                    setFileopen(false);
                    file->close();
                    is_first_line = true;
                    received_bytes = 0;
                    emit fileReceivingDone();
                    continue;
                }
                file->write(m_message);
                received_bytes += m_message.length();
                emit bytesReceivedSignal(received_bytes);
                qDebug() << "Progress:" << received_bytes << "/" << bytes_to_receive << "\r\n";
            }
            m_message.clear();
            //emit parseCallback(buffer);
        }
    }
    emit finished();
}

void UARTWorker::setRunning(bool running){
    if (m_running == running)
        return;

    m_running = running;
    emit runningChanged(m_running);
}

void UARTWorker::setMessage(QByteArray message){
    QMutexLocker lock(&mutex);
    if (m_message == message)
        return;

    m_message = message;
    emit messageChanged(m_message);
}

void UARTWorker::setFileopen(bool fileopen)
{
    if (m_fileopen == fileopen)
        return;

    m_fileopen = fileopen;
    emit fileopenChanged(m_fileopen);
}

void UARTWorker::openFile(QString fileName){
    setFileopen(true);
    file = new QFile(fileName);
    if (!file->open(QIODevice::WriteOnly)) {
        //ui->t_cmd->append(QString("Error while opening: %1").arg(fileName));
        return;
    }
}
