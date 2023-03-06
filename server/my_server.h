#pragma once
#include <SDKDDKVer.h>
#include <iostream>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include "ser_des.h"

using boost::asio::ip::tcp;

struct MyServer
{
    static const int max_length = 100*1024;

    void session(tcp::socket sock)
    {
        boost::asio::streambuf buf;
        size_t already_read_ = 0;
        DirListType dir;
        SerDes::TypeCmd cmd;
        size_t total_size = 0;
        try
        {
            for (;;)
            {
                boost::system::error_code error;
                
                already_read_ += boost::asio::read(sock, buf, error);
                
                if (already_read_ >= (sizeof(SerDes::TypeCmd) + sizeof(size_t)) && !total_size)
                {
                    total_size = SerDes::deserialize(buf, dir, cmd, !total_size);
                    already_read_ -= sizeof(size_t) * 2;
                }

                if (already_read_ >= total_size)
                {
                    SerDes::deserialize(buf, dir, cmd);

                    std::cout << "get list of dir = '" << dir.first << "'" << std::endl;
                    
                    GetFilesList::getInstance().getFilesList("D:/", dir);
                    std::cout << "--------------------" << std::endl;
                    std::cout << "CMD = " << static_cast<unsigned long long>(cmd) << std::endl;
                    GetFilesList::printIt(dir);

                    already_read_ = 0;
                    total_size = 0;

                    size_t request_length = SerDes::serialize(buf, dir, cmd);

                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    boost::asio::write(sock, buf);
                }

                if (error == boost::asio::error::eof)
                    break;
                else if (error)
                    throw boost::system::system_error(error);
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