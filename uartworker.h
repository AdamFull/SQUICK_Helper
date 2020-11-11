#ifndef UARTWORKER_H
#define UARTWORKER_H

#include <QThread>
#include <QObject>
#include <QMutex>
#include <QFile>

class UARTWorker : public QObject{
    Q_OBJECT
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(QByteArray message READ message WRITE setMessage NOTIFY messageChanged)
    Q_PROPERTY(bool fileopen READ fileopen WRITE setFileopen NOTIFY fileopenChanged);
    bool m_running;

    QByteArray m_message;

public:
    UARTWorker(QObject *parent = 0);
    ~UARTWorker();
    bool running() const;
    QByteArray message() const;

    bool fileopen() const;
    void openFile(QString fileName);

public slots:
    void run();
    void setRunning(bool running);
    void setMessage(QByteArray message);

    void setFileopen(bool fileopen);

signals:
    void finished();
    void runningChanged(bool running);
    void messageChanged(QByteArray message);

    void fileopenChanged(bool fileopen);

    void bytesToReceiveSignal(int bytes_to_receive);
    void bytesReceivedSignal(int bytes_received);

    void fileReceivingDone();

private:
    QMutex mutex;
    QThread workerThread;
    QFile *file;
    bool m_fileopen;
};

#endif // UARTWORKER_H
