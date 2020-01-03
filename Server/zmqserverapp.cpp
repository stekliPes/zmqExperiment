#include "zmqserverapp.h"

ZmqServerApp::ZmqServerApp(int argc, char *argv[]):
    QCoreApplication(argc,argv),
    m_zmqContext(),
    m_repSocket(m_zmqContext,zmq::socket_type::rep),
    m_pubSocket(m_zmqContext,zmq::socket_type::pub),
    m_subSocket(m_zmqContext,zmq::socket_type::sub),
    m_repNotifier(m_repSocket.getsockopt<qintptr>(ZMQ_FD),QSocketNotifier::Read),
    m_subNotifier(m_subSocket.getsockopt<qintptr>(ZMQ_FD),QSocketNotifier::Read)
{

}

ZmqServerApp::~ZmqServerApp()
{
    m_pubSocket.close();
    m_subSocket.close();
    m_repSocket.close();
}
