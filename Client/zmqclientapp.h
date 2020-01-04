#ifndef ZMQCLIENTAPP_H
#define ZMQCLIENTAPP_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSocketNotifier>

#include "zmq.hpp"

class ZmqClientApp : public QGuiApplication
{
    Q_OBJECT
    Q_PROPERTY(bool lights READ lights NOTIFY lightsChanged)
public:
    explicit ZmqClientApp(int argc, char *argv[]);
    virtual ~ZmqClientApp();

    Q_INVOKABLE void startSending();
    Q_INVOKABLE void stopSending();
    Q_INVOKABLE void setInterval(int);
    Q_INVOKABLE void toggleLights();

    inline bool lights() const
    {
        return m_lights;
    }

signals:
    void lightsChanged(bool);

private slots:
    void subData();

private:
    void setupPublisher();
    void setupRequester();
    void setupSubscriber();

    int events(zmq::socket_t &);

    QQmlApplicationEngine m_qmlEngine;

    zmq::context_t m_zmqContext;
    zmq::socket_t m_reqSocket;
    zmq::socket_t m_pubSocket;
    zmq::socket_t m_subSocket;

    QSocketNotifier m_subNotifier;

    QString m_serverAddress;

    bool m_lights;
};

#endif // ZMQCLIENTAPP_H
