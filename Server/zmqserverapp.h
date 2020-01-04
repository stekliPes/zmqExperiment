#ifndef ZMQSERVERAPP_H
#define ZMQSERVERAPP_H

#include <QCoreApplication>

#include <QSocketNotifier>
#include <QTimer>

#include "zmq.hpp"

#include <iostream>

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
    void setupPublisher();
    void setupResponder();
    void setupSubscriber();

    int events(zmq::socket_t &);

    inline void toggleLights();
    inline void setResponseWord(const QString &);
    inline void startPublish();
    inline void stopPublish();
    inline void setInterval(const int msec);

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

void ZmqServerApp::toggleLights()
{
    m_lightsOn = !m_lightsOn;
}

void ZmqServerApp::setResponseWord(const QString &word)
{
    m_responseWord = word;
}

void ZmqServerApp::startPublish()
{
    std::cout << "Starting publishing" << std::endl;
    m_publishTimer.start();
}

void ZmqServerApp::stopPublish()
{
    std::cout << "Stopping publishing" << std::endl;
    m_publishTimer.stop();
}

void ZmqServerApp::setInterval(const int msec)
{
    std::cout << "Setting publishing interval to " << msec << std::endl;
    m_publishTimer.setInterval(msec);
}



#endif // ZMQSERVERAPP_H
