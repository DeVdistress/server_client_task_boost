#pragma once
#include <SDKDDKVer.h>
#include "get_files_list.h"
#include <boost/asio.hpp>

struct SerDes
{
    enum class TypeCmd : unsigned long long
    {
        request_dir = 0xFFA07502,
        submit_dir  = 0x55A07501,
    };

    inline static const size_t serialize(boost::asio::streambuf& buf, const DirListType& wt, const TypeCmd& cmd) {
        std::ostream out(&buf);

        unsigned long long my_cmd = static_cast<unsigned long long>(cmd);
        out.write(reinterpret_cast<const char*>(&my_cmd), sizeof(my_cmd));

        size_t total_size = 0;

        if (cmd == TypeCmd::submit_dir) {
            total_size = sizeof(wt);
        }
        else if (cmd == TypeCmd::request_dir) {
            total_size = sizeof(my_cmd);
        }
            
        out.write(reinterpret_cast<const char*>(&total_size), sizeof(total_size));

        if (cmd != TypeCmd::request_dir) {
            size_t first_size = wt.first.size();
            size_t second_size = wt.second.size();

            out.write(reinterpret_cast<const char*>(&first_size), sizeof(first_size));
            out.write(reinterpret_cast<const char*>(&second_size), sizeof(second_size));
            out << wt.first << std::endl;
            out << wt.second << std::endl;
        }

        return total_size;
    }

    inline static const size_t deserialize(boost::asio::streambuf& buf, DirListType& wt, TypeCmd& cmd) {
        std::istream is(&buf);

        unsigned long long my_cmd = static_cast<unsigned long long>(cmd);
        size_t total_size = 0;
        size_t first_size_2 = 0;
        size_t second_size_2 = 0;

        is.read(reinterpret_cast<char*>(&my_cmd), sizeof(my_cmd));
        is.read(reinterpret_cast<char*>(&total_size), sizeof(total_size));

        cmd = static_cast<TypeCmd>(my_cmd);

        if (cmd != TypeCmd::request_dir) {
            is.read(reinterpret_cast<char*>(&first_size_2), sizeof(first_size_2));
            is.read(reinterpret_cast<char*>(&second_size_2), sizeof(second_size_2));
            wt.first.resize(first_size_2);
            wt.second.resize(second_size_2);
            is >> wt.first;
            is >> wt.second;
        }

        return total_size;
    }
};

