#pragma once
#include <SDKDDKVer.h>
#include <iostream>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include "ser_des.h"

using boost::asio::ip::tcp;

class MyServer
{
    static const int max_length = 100*1024;

    void session(tcp::socket sock)
    {
        boost::asio::streambuf buf;
        size_t already_read_ = 0;
        DirListType dir;
        SerDes::TypeCmd cmd; 
        try
        {
            for (;;)
            {
                boost::system::error_code error;
                boost::asio::streambuf::mutable_buffers_type mutableBuffer = buf.prepare(max_length);

                already_read_ += sock.read_some(mutableBuffer, error);
                auto sz_cmd = SerDes::deserialize(buf, dir, cmd);

                if (cmd == SerDes::TypeCmd::request_dir &&
                    sz_cmd == sizeof(SerDes::TypeCmd::request_dir)) {
                    std::cout << "WOW!!!!!!!!!" << std::endl;
                }

                if (error == boost::asio::error::eof)
                    break; // Connection closed cleanly by peer.
                else if (error)
                    throw boost::system::system_error(error); // Some other error.
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

