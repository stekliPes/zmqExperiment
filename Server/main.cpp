#include <QCoreApplication>

#include <QThread>

#include "zmqserverapp.h"

int main(int argc, char *argv[])
{
    ZmqServerApp a(argc,argv);

    return a.exec();
}
