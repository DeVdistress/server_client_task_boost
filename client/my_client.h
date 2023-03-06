#pragma once
#include <SDKDDKVer.h>
#include <ostream>
#include <sstream> 
#include <boost/asio.hpp>
#include "ser_des.h"


using boost::asio::ip::tcp;

struct MyClient
{
    static const int max_length = 100 * 1024;

    std::string getPathFromConsole() {

        while (true) {
            std::cout << "Enter message: ";
            std::string consol;
            std::cin >> consol;

            const fs::path path(consol);
            std::error_code ec;
            if (fs::is_directory(path, ec))
                return consol;
        }
    }

    int work(char *host, char* port)
    {
        boost::asio::streambuf buf_request;
        boost::asio::streambuf buf_reply;

        try
        {
            boost::asio::io_context io_context;

            tcp::socket s(io_context);
            tcp::resolver resolver(io_context);
            boost::asio::connect(s, resolver.resolve(host, port));

            std::string path = getPathFromConsole();

            DirListType tmp;
            SerDes::TypeCmd cmd = SerDes::TypeCmd::request_dir;
            size_t request_length = SerDes::serialize(buf_request, tmp, cmd);
            boost::asio::streambuf::mutable_buffers_type mutableBuffer_req = buf_request.prepare(max_length);
            boost::asio::write(s, boost::asio::buffer(mutableBuffer_req, request_length));

/*
            boost::asio::streambuf::mutable_buffers_type mutableBuffer_rep = buf_reply.prepare(max_length);
            size_t reply_length = boost::asio::read(s,
                boost::asio::buffer(reply, request_length));
            std::cout << "Reply is: ";
            std::cout.write(reply, reply_length);
            std::cout << "\n";
 */
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << "\n";
        }

        return 0;
    }
};

