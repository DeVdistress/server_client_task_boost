// server.cpp
//
#include <iostream>
#include <iterator>
#include <algorithm>
#include "my_server.h"
#include "get_files_list.h"


#if 1
int main()
{
    DirListType tmp;

    GetFilesList::getInstance().getFilesList("D:/r200", tmp);
    GetFilesList::printIt(tmp);
    GetFilesList::getInstance().getFilesList("D:/", tmp);
    GetFilesList::printIt(tmp);
    GetFilesList::getInstance().getFilesList("D:/r200", tmp);
    GetFilesList::printIt(tmp);

    return 0;
}
#endif