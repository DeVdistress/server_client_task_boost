// server.cpp
//
#include <iostream>
#include <iterator>
#include <algorithm>

#include "my_server.h"
#include "get_files_list.h"

#define TEST_IS_WORKING
#define THIS_ONE_HAS_MAIN

void testFileList() {
    DirListType tmp;
    GetFilesList::getInstance().getFilesList("D:/", tmp);
    GetFilesList::printIt(tmp);
    GetFilesList::getInstance().getFilesList("D:/income", tmp);
    GetFilesList::printIt(tmp);
    GetFilesList::getInstance().getFilesList("D:/install", tmp);
    GetFilesList::printIt(tmp);
    GetFilesList::getInstance().getFilesList("E:/install", tmp);
    GetFilesList::printIt(tmp);
    GetFilesList::getInstance().getFilesList("D:/", tmp);
    GetFilesList::printIt(tmp);
    GetFilesList::getInstance().getFilesList("C:/boost/boost_1_81_0", tmp);
    GetFilesList::printIt(tmp);
    GetFilesList::getInstance().getFilesList("C:/Python/Python39", tmp);
    GetFilesList::printIt(tmp);

    boost::asio::streambuf buf_out;

    // Serialize
    std::ostream out(&buf_out);
    size_t first_size = tmp.first.size(), second_size = tmp.second.size();
    out.write(reinterpret_cast<const char*>(&first_size), sizeof(first_size));
    out.write(reinterpret_cast<const char*>(&second_size), sizeof(second_size));
    out << tmp.first << std::endl;
    out << tmp.second << std::endl;


    // Deserialize
    std::istream is(&buf_out);
    DirListType tmp2;
    size_t first_size_2 = 0, second_size_2 = 0;
    is.read(reinterpret_cast<char*>(&first_size_2), sizeof(first_size_2));
    is.read(reinterpret_cast<char*>(&second_size_2), sizeof(second_size_2));
    tmp2.first.resize(first_size_2);
    tmp2.second.resize(second_size_2);
    is >> tmp2.first;
    is >> tmp2.second;

    GetFilesList::printIt(tmp2);
}

#if defined(THIS_ONE_HAS_MAIN)
    int main(int argc, char* argv[])
    {
        int port = 0;

    #if defined(TEST_IS_WORKING)
        testFileList();
    #endif

        if (argc != 2)
        {
            std::cerr << "Usage: server <port>\n";
            return 1;
        }
        
        port = std::atoi(argv[1]);
    
        return 0;
    }
#endif
#undef THIS_ONE_HAS_MAIN
#undef TEST_IS_WORKING