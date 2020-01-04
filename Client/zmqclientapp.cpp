#include "zmqclientapp.h"

#include <iostream>

#include <QQmlContext>

ZmqClientApp::ZmqClientApp(int argc, char *argv[]):
    QGuiApplication(argc, argv),
    m_qmlEngine(QStringLiteral("qrc:/main.qml")),
    m_zmqContext(),
    m_reqSocket(m_zmqContext,zmq::socket_type::req),
    m_pubSocket(m_zmqContext,zmq::socket_type::pub),
    m_subSocket(m_zmqContext,zmq::socket_type::sub),
    m_subNotifier(m_subSocket.getsockopt<qintptr>(ZMQ_FD),QSocketNotifier::Read),
    m_serverAddress(arguments().size()>1 ? arguments().at(1) : "localhost"),
    m_lights(false)
{
    std::cout << "Connecting to " << m_serverAddress.toStdString() << std::endl;
    m_qmlEngine.rootContext()->setContextProperty("app",this);
    setupPublisher();
    setupRequester();
    setupSubscriber();
}

ZmqClientApp::~ZmqClientApp()
{

}

void ZmqClientApp::startSending()
{
    zmq::message_t msg;
    std::cout << "Requesting start sendding " << std::endl;
    m_reqSocket.send("Start",5,static_cast<int>(zmq::send_flags::none));
    std::cout << "Command sent";
    m_reqSocket.recv(msg);
    std::cout << "Response is " << std::string((char*)msg.data(),msg.size()) << std::endl;
}

void ZmqClientApp::stopSending()
{
    zmq::message_t msg;
    std::cout << "Requesting stop sendding " << std::endl;
    m_reqSocket.send("Stop",4,static_cast<int>(zmq::send_flags::none));
    std::cout << "Command sent";
    m_reqSocket.recv(msg);
    std::cout << "Response is " << std::string((char*)msg.data(),msg.size()) << std::endl;
}

void ZmqClientApp::setInterval(int interval)
{
    zmq::message_t msg;
    std::cout << "Requesting interval of " << interval <<  std::endl;
    m_reqSocket.send("Interval",8,static_cast<int>(zmq::send_flags::sndmore));
    m_reqSocket.send((void*)&interval,sizeof(int),static_cast<int>(zmq::send_flags::none));
    std::cout << "Command sent";
    m_reqSocket.recv(msg);
    std::cout << "Response is " << std::string((char*)msg.data(),msg.size()) << std::endl;
}

void ZmqClientApp::toggleLights()
{
    zmq::message_t msg;
    std::cout << "Requesting lights toggle " << std::endl;
    m_reqSocket.send("Lights",6,static_cast<int>(zmq::send_flags::none));
    std::cout << "Command sent";
    m_reqSocket.recv(msg);
    std::cout << "Response is " << std::string((char*)msg.data(),msg.size()) << std::endl;
}

void ZmqClientApp::subData()
{
    m_subNotifier.setEnabled(false);
    while(events(m_subSocket) & ZMQ_POLLIN)
    {
    std::cout << "Receiving data..." << std::endl;

    zmq::message_t message;
    int i=0;
    do
    {
        m_subSocket.recv(message);
        QString data = QString::fromStdString(std::string((char*)message.data(),message.size()));
        std::cout << "Part " << i++ << ":\t" << data.toStdString() << std::endl;
        if(data.startsWith("Lights"))
        {
            bool newLightsOn = data.contains("On") ? true : false;
            if(newLightsOn != m_lights)
            {
                m_lights = newLightsOn;
                emit lightsChanged(m_lights);
            }
        }
    }
    while(message.more());
    std::cout << "End" << std::endl;
    }
    m_subNotifier.setEnabled(true);
}

void ZmqClientApp::setupPublisher()
{

}

void ZmqClientApp::setupRequester()
{
    m_reqSocket.connect(QString("tcp://%1:6000").arg(m_serverAddress).toStdString());
}

void ZmqClientApp::setupSubscriber()
{
    m_subSocket.connect(QString("tcp://%1:5000").arg(m_serverAddress).toStdString());
    m_subSocket.setsockopt(ZMQ_SUBSCRIBE,"",0);
    connect
    (
        &m_subNotifier,
        &QSocketNotifier::activated,
        this,
        &ZmqClientApp::subData
    );
    std::cout << "Subscriber started on port 5000" << std::endl;
}

int ZmqClientApp::events(zmq::socket_t &socket)
{
    int events = 0;
    std::size_t eventsSize = sizeof(events);
    zmq_getsockopt(socket,ZMQ_EVENTS, &events, &eventsSize);
    return events;
}
