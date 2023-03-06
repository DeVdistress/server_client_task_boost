// server.cpp
//
#include <iterator>
#include <algorithm>

#include "my_server.h"
#include "get_files_list.h"
#include "ser_des.h"

//#define TEST_IS_WORKING
#define THIS_ONE_HAS_MAIN

void testFileList() {
    DirListType tmp, tmp2;
    GetFilesList::getInstance().getFilesList("D:/", tmp);
    //GetFilesList::printIt(tmp);
    GetFilesList::getInstance().getFilesList("D:/", tmp);
    //GetFilesList::printIt(tmp);
    GetFilesList::getInstance().getFilesList("C:/boost/boost_1_81_0", tmp);
    //GetFilesList::printIt(tmp);
    GetFilesList::getInstance().getFilesList("C:/Python/Python39", tmp);
    GetFilesList::printIt(tmp);

    boost::asio::streambuf buf;
    SerDes::TypeCmd cmd = SerDes::TypeCmd::request_dir;
    SerDes::serialize(buf, tmp, cmd);

    SerDes::deserialize(buf, tmp2, cmd);
    std::cout << "CMD = " << static_cast<unsigned long long>(cmd) << std::endl;
    GetFilesList::printIt(tmp2);

    cmd = SerDes::TypeCmd::submit_dir;
    SerDes::serialize(buf, tmp, cmd);

    SerDes::deserialize(buf, tmp2, cmd);
    std::cout << "CMD = " << static_cast<unsigned long long>(cmd) << std::endl;
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