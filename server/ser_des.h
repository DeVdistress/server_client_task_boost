#pragma once
#include <SDKDDKVer.h>
#include "get_files_list.h"
#include <boost/asio.hpp>

struct SerDes
{
    enum class TypeCmd : size_t
    {
        request_dir = 0xFFA07502,
        submit_dir  = 0x55A07501,
    };

    inline static const size_t serialize(boost::asio::streambuf& buf, const DirListType& wt, const TypeCmd& cmd) {
        std::ostream out(&buf);
        size_t total_size = 0;

        // Cmd
        size_t my_cmd = static_cast<size_t>(cmd);
        out.write(reinterpret_cast<const char*>(&my_cmd), sizeof(my_cmd));

        // Total size
        total_size = sizeof(my_cmd) + sizeof(total_size) +
            sizeof(size_t) + wt.first.size();

        if (cmd != TypeCmd::request_dir) {
            total_size += sizeof(size_t) + sizeof(wt.second);
        }

        out.write(reinterpret_cast<const char*>(&total_size), sizeof(total_size));
        
        // First size
        size_t first_size = wt.first.size();
        out.write(reinterpret_cast<const char*>(&first_size), sizeof(first_size));
        
        // First
        out << wt.first << std::endl;
   
        if (cmd != TypeCmd::request_dir) {
            // Second size
            size_t second_size = wt.second.size();
            out.write(reinterpret_cast<const char*>(&second_size), sizeof(second_size));
                        
            // Second
            out << wt.second << std::endl;
        }

        return total_size;
    }

    inline static const size_t deserialize(boost::asio::streambuf& buf, DirListType& wt, TypeCmd& cmd, bool only_cmd_and_size = false) {
        std::istream is(&buf);

        size_t my_cmd = static_cast<size_t>(cmd);
        size_t total_size = 0;
        size_t first_size = 0;
        size_t second_size = 0;
        size_t true_total_size = 0;
        char tmp;

        if (only_cmd_and_size) {
            // Cmd
            is.read(reinterpret_cast<char*>(&my_cmd), sizeof(my_cmd));
            total_size += sizeof(my_cmd);
            cmd = static_cast<TypeCmd>(my_cmd);

            // Total size
            is.read(reinterpret_cast<char*>(&true_total_size), sizeof(true_total_size));
            total_size += sizeof(total_size);

            return true_total_size - total_size;
        }
        else 
        {
            // First size
            is.read(reinterpret_cast<char*>(&first_size), sizeof(first_size));
            total_size += sizeof(first_size);

            // First
            wt.first.resize(first_size);
            is >> wt.first;
            total_size += sizeof(wt.first);
            is.get(tmp);

            cmd = static_cast<TypeCmd>(my_cmd);
            if (cmd != TypeCmd::request_dir) {
                // Second size
                is.read(reinterpret_cast<char*>(&second_size), sizeof(second_size));
                total_size += sizeof(second_size);

                // Second
                wt.second.resize(second_size);
                is >> wt.second;
                total_size += sizeof(wt.second);
                is.get(tmp);
            }
        }

        /*
        if (true_total_size != total_size) {
            std::cout << "Error true_total_size=" << true_total_size <<
                "  != total_size=" << total_size << std::endl;
        }
        */
        return total_size;
    }
};

