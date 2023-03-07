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
#if(1)
    static const int max_length = 100 * 1024;
    boost::asio::io_context io_context;
    tcp::socket s;
    tcp::resolver resolver;

    MyClient() : io_context(), s(io_context), resolver(io_context) {}

    std::string getPathFromConsole()
    {

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

    size_t up_to_enter(const boost::system::error_code&, size_t bytes)
    {
        for (size_t i = 0; i < bytes; ++i)
            //if (buff[i + offset] == '\n')
            //    return 0;
            return 1;
    }

    void on_read(const boost::system::error_code&, size_t) {}

    int work(char* host, char* port)
    {
        boost::asio::streambuf buf;
        boost::asio::streambuf buf_r;

        try
        {
            boost::asio::connect(s, resolver.resolve(host, port));

            size_t already_read_ = 0;
            DirListType dir;
            size_t total_size = 0;

            while (true)
            {
                std::string path = getPathFromConsole();

                DirListType tmp;
                tmp.first = path;
                SerDes::TypeCmd cmd = SerDes::TypeCmd::request_dir;

                size_t request_length = SerDes::serialize(buf, tmp, cmd);
                boost::asio::write(s, buf);

                while (true)
                {
                    boost::system::error_code error;
                    already_read_ += boost::asio::read(s, buf_r, error);

                    if (already_read_ >= (sizeof(SerDes::TypeCmd) + sizeof(size_t)) && !total_size)
                    {
                        total_size = SerDes::deserialize(buf, dir, cmd, !total_size);
                        already_read_ -= sizeof(size_t) * 2;
                    }

                    if (already_read_ != 0 && already_read_ >= total_size)
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

        return 0;
    }

#endif
};


