#pragma once
#include <SDKDDKVer.h>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include <chrono>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class MyServer
{
    static const int max_length = 1024;

    void session(tcp::socket sock)
    {
        try
        {
            for (;;)
            {
                char data[max_length];

                boost::system::error_code error;
                size_t length = sock.read_some(boost::asio::buffer(data), error);
                if (error == boost::asio::error::eof)
                    break; // Connection closed cleanly by peer.
                else if (error)
                    throw boost::system::system_error(error); // Some other error.

                boost::asio::write(sock, boost::asio::buffer(data, length));
            }
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception in thread: " << e.what() << "\n";
        }
    }

    void server(boost::asio::io_context& io_context, unsigned short port)
    {
        boost::asio::ip::tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
        for (;;)
        {
            auto my_thread = std::thread(&MyServer::session, this, a.accept());
            my_thread.detach();
        }
    }

    int work(int port)
    {
        try
        {
            boost::asio::io_context io_context;

            server(io_context, port);
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << "\n";
            std::cout << "Exception: " << e.what() << "\n";
        }

        return 0;
    }
};

