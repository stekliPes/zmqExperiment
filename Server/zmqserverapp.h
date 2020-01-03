#ifndef ZMQSERVERAPP_H
#define ZMQSERVERAPP_H

#include <QCoreApplication>

#include <QSocketNotifier>
#include <QTimer>

#include "zmq.hpp"

class ZmqServerApp : public QCoreApplication
{
    Q_OBJECT
public:
    explicit ZmqServerApp(int argc, char *argv[]);
    virtual ~ZmqServerApp();

private slots:
    void repData();
    void subData();
    void pubData();

private:
    void toggleLights();
    void setResponseWord(const QString &);
    void startPublish();
    void stopPublish();
    void setInterval();


    zmq::context_t m_zmqContext;
    zmq::socket_t m_repSocket;
    zmq::socket_t m_pubSocket;
    zmq::socket_t m_subSocket;

    QSocketNotifier m_repNotifier;
    QSocketNotifier m_subNotifier;

    QString m_responseWord;
    bool m_lightsOn;

    QTimer m_publishTimer;
};

#endif // ZMQSERVERAPP_H
