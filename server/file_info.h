#pragma once
#include <string>
#include <vector>
#include <map>
#include <ostream>
#include <sstream> 

struct FileInfo
{
	enum class TypeFile : char
    {
        dir = 'd',
        file = 'f',
        symlink = 's',
        unknown = 'u'
    };

    inline static const std::map<FileInfo::TypeFile, const char*> enum_strings {
            { FileInfo::TypeFile::dir, "(dir)" },
            { FileInfo::TypeFile::file, "(file)" },
            { FileInfo::TypeFile::symlink, "(link)" }
    };

    static std::string getFileTypeString(FileInfo::TypeFile file_enum)
    {
        auto it = enum_strings.find(file_enum);
        return it == enum_strings.end() ? "(unknown)" : it->second;
    }

    std::string name;
    std::string type;
    FileInfo::TypeFile enum_type;
	size_t size_of_file = 0;

    friend std::ostream& operator<<(std::ostream& os, const FileInfo& data) {
        os << data.name << std::endl;
        os << data.type << std::endl;
        os.put(static_cast<char>(data.enum_type));
        os.write(reinterpret_cast<const char*>(&data.size_of_file), sizeof(data.size_of_file));
        return os;
    }

    friend std::ostream& operator<<(std::ostream& os, const std::vector<FileInfo>& data) {
        for (auto i : data)
            os << i;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, FileInfo::TypeFile& data)
    {
        char tmp;
        is.get(tmp);
        is.get(tmp);
        data = static_cast<FileInfo::TypeFile>(tmp);
        return is;
    }

    friend std::istream& operator>>(std::istream& is, FileInfo& data) {
        is >> data.name >> data.type >> data.enum_type;
        is.read(reinterpret_cast<char*>(&data.size_of_file), sizeof(data.size_of_file));
        return is;
    }

    friend std::istream& operator>>(std::istream& is, std::vector<FileInfo>& data) {
        for (auto& i : data)
            is >> i;
        return is;
    }
};