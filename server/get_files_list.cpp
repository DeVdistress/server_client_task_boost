#include "get_files_list.h"

bool GetFilesList::exploreFolder(std::string& name)
{
	bool res = false;
    const std::string pathString = name;
    const fs::path path(pathString);
    std::error_code ec;

    if (fs::is_directory(path, ec))
    {
        for (auto const& dir_entry : fs::recursive_directory_iterator("sandbox"))
        {
            std::cout << dir_entry << '\n';
        }


        //HumanReadable{fs::file_size(p)}

        res = true;
    }

    if (ec)
    {
        std::cerr << "Error in is_directory: " << ec.message();
    }
	return res;
}

bool GetFilesList::getFileList()
{
	bool res = false;

	return res;
}

bool GetFilesList::getFileStatus(std::string& name, FileInfo& f)
{	
    bool res = false;

    fs::file_status s = fs::status(name);

    if (fs::is_regular_file(s)){ 
        f.enum_type = FileInfo::TypeFile::file;
        f.size_of_file = fs::file_size(name);
    }
    else if (fs::is_directory(s)) {
        f.enum_type = FileInfo::TypeFile::dir;
    }
    else if (fs::is_symlink(s)) {
        f.enum_type = FileInfo::TypeFile::symlink;
    }
    else {
        f.enum_type = FileInfo::TypeFile::unknown;
    }

    f.type = FileInfo::getFileTypeString(f.enum_type);

    //if (!fs::exists(s)) std::cout << " does not exist\n";
	return res;
}