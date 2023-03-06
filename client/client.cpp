// client.cpp
//
#include <iostream>
#include <string>
#include "my_client.h"

#if(1)
int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
        return 1;
    }

    char *host = argv[1];
    char *port = argv[2];

    MyClient cl;
    cl.work(host, port);


    return 0;
}
#endif