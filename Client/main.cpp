#include "zmqclientapp.h"
int main(int argc, char *argv[])
{
    return ZmqClientApp(argc,argv).exec();
}
