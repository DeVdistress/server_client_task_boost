#pragma once
#include <SDKDDKVer.h>
#include <ostream>
#include <sstream>
#include <thread>
#include <boost/asio.hpp>
#include "ser_des.h"
#include "get_files_list.h"
#include "single_tone.h"

using boost::asio::ip::tcp;

struct MyClient
{
    static const int max_length = 100 * 1024;

    std::string getPathFromConsole() {

        while (true) {
            std::cout << "Enter message: ";
            std::string consol;
            std::cin >> consol;

            const std::filesystem::path path(consol);
            std::error_code ec;
            if (std::filesystem::is_directory(path, ec))
                return consol;
        }
    }

    void client(boost::asio::io_context& io_context, tcp::socket& s, tcp::resolver& resolver, char* host, char* port)
    {
        auto my_thread = std::thread(&MyClient::session, this, std::ref(s), std::ref(resolver), host, port);
        //my_thread.detach();
        my_thread.join();
    }

    int work(char* host, char* port)
    {
        boost::asio::io_context io_context;

        tcp::socket s(io_context);
        tcp::resolver resolver(io_context);
        
        client(io_context, s, resolver, host, port);

        return 0;
    }

    void session(tcp::socket& s, tcp::resolver& resolver, char* host, char* port)
    {
        boost::asio::streambuf buf;

        boost::asio::connect(s, resolver.resolve(host, port));

        try
        {
            size_t already_read_ = 0;
            DirListType dir;
            size_t total_size = 0;

            while (true) {
                std::string path = getPathFromConsole();

                DirListType tmp;
                tmp.first = path;
                SerDes::TypeCmd cmd = SerDes::TypeCmd::request_dir;

                size_t request_length = SerDes::serialize(buf, tmp, cmd);
                boost::asio::write(s, buf);

                while (false) {
                    
                    boost::system::error_code error;
                    already_read_ += boost::asio::read(s, buf, error);

                    if (already_read_ >= (sizeof(SerDes::TypeCmd) + sizeof(size_t)) && !total_size)
                    {
                        total_size = SerDes::deserialize(buf, dir, cmd, !total_size);
                        already_read_ -= sizeof(size_t) * 2;
                    }

                    if (already_read_ >= total_size)
                    {
                        SerDes::deserialize(buf, dir, cmd);

                        std::cout << "get list of dir = '" << dir.first << "'" << std::endl;

                        std::cout << "--------------------" << std::endl;
                        std::cout << "CMD = " << static_cast<unsigned long long>(cmd) << std::endl;
                        GetFilesList::printIt(dir);

                        already_read_ = 0;
                        total_size = 0;

                        break;
                    }

                    if (error == boost::asio::error::eof)
                        break;
                    else if (error)
                        throw boost::system::system_error(error);
                }
            }

        }
        catch (std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << "\n";
        }
    }
};

