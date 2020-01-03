#include "zmqserverapp.h"

#include <iostream>

ZmqServerApp::ZmqServerApp(int argc, char *argv[]):
    QCoreApplication(argc,argv),
    m_zmqContext(),
    m_repSocket(m_zmqContext,zmq::socket_type::rep),
    m_pubSocket(m_zmqContext,zmq::socket_type::pub),
    m_subSocket(m_zmqContext,zmq::socket_type::sub),
    m_repNotifier(m_repSocket.getsockopt<qintptr>(ZMQ_FD),QSocketNotifier::Read),
    m_subNotifier(m_subSocket.getsockopt<qintptr>(ZMQ_FD),QSocketNotifier::Read),
    m_responseWord("Difolt"),
    m_lightsOn(false),
    m_publishTimer()
{
    setupPublisher();
    setupResponder();
    setupSubscriber();
}

ZmqServerApp::~ZmqServerApp()
{
    m_pubSocket.close();
    m_subSocket.close();
    m_repSocket.close();
}

void ZmqServerApp::repData()
{

}

void ZmqServerApp::subData()
{

}

void ZmqServerApp::pubData()
{
    std::ostringstream part1, part2, part3;
    part1 << "Server status: ";
    part2 << "Config: \t" << m_responseWord.toStdString();
    part3 << "Lights: \t" << (m_lightsOn ? "On" : "Off");

    m_pubSocket.send(part1.str().c_str(),part1.str().length(),static_cast<int>(zmq::send_flags::sndmore));
    m_pubSocket.send(part2.str().c_str(),part2.str().length(),static_cast<int>(zmq::send_flags::sndmore));
    m_pubSocket.send(part3.str().c_str(),part3.str().length(),static_cast<int>(zmq::send_flags::none));
}

void ZmqServerApp::setupPublisher()
{
    m_pubSocket.bind("tcp://*:5000");
    m_publishTimer.setInterval(1000);
    connect
    (
        &m_publishTimer,
        &QTimer::timeout,
        this,
        &ZmqServerApp::pubData
    );
    m_publishTimer.start();
    std::cout << "Publisher started on port 5000, interval of 1000 ms" << std::endl;
}

void ZmqServerApp::setupResponder()
{
    m_repSocket.bind("tcp://*:6000");
    connect
    (
        &m_repNotifier,
        &QSocketNotifier::activated,
        this,
        &ZmqServerApp::repData
    );
    std::cout << "Responder started on port 6000" << std::endl;
}

void ZmqServerApp::setupSubscriber()
{
    m_subSocket.bind("tcp://*:7000");
    connect
    (
        &m_subNotifier,
        &QSocketNotifier::activated,
        this,
        &ZmqServerApp::subData
    );
    std::cout << "Subscriber started on port 7000" << std::endl;
}


