#pragma once
#include <string>
#include <map>

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
};