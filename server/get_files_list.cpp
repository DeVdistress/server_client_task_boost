#include "get_files_list.h"

bool GetFilesList::exploreFolder(const std::string& name)
{
	bool res = false;
    const std::string pathString = name;
    const fs::path path(pathString);
    std::error_code ec;

    DirListType tmp;
    
    if (fs::is_directory(path, ec))
    {
        tmp.first = path.string();

        if (!this->cash.isValid(tmp.first)) {

            for (auto const& entry : fs::directory_iterator(path)) {
                FileInfo tmp_f_i;
                getFileStatus(entry.path(), tmp_f_i);
                tmp.second.emplace_back(std::move(tmp_f_i));
            }

            this->cash.setList(tmp);
        }
        //HumanReadable{fs::file_size(p)}
        res = true;
    }

    if (ec)
    {
        std::cerr << "Error in is_directory: " << ec.message();
        std::cout << "Error in is_directory: " << ec.message();
    }
	return res;
}

bool GetFilesList::getFilesList(const std::string& name, DirListType& out_list)
{
	bool res = false;
    res = exploreFolder(name);
    
    if (res) {
        out_list.first = name;
        res = cash.getList(out_list);
    }
	return res;
}

bool GetFilesList::getFileStatus(const fs::path& name, FileInfo& f)
{	
    bool res = false;

    fs::file_status s = fs::status(name);

    if (fs::is_regular_file(s)){ 
        f.enum_type = FileInfo::TypeFile::file;
        f.size_of_file = fs::file_size(name);
        f.name = name.filename().string();      
    }
    else if (fs::is_directory(s)) {
        f.enum_type = FileInfo::TypeFile::dir;
        f.name = name.filename().string();
    }
    else if (fs::is_symlink(s)) {
        f.enum_type = FileInfo::TypeFile::symlink;
        f.name = name.filename().string();
    }
    else {
        f.enum_type = FileInfo::TypeFile::unknown;
    }

    f.type = FileInfo::getFileTypeString(f.enum_type);

    //if (!fs::exists(s)) std::cout << " does not exist\n";
	return res;
}

void GetFilesList::printIt(const DirListType& stuff)
{
    std::cout << "--------------------" << std::endl;
    std::cout << stuff.first << std::endl;

    for (auto i : stuff.second) {
        std::cout << " " << i.name << " " << i.type 
            << " " << HumanReadable{i.size_of_file} << std::endl;
    }
}