#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "file_info.h"
#include "time_cash.h"
#include "single_tone.h"

struct HumanReadable {
    std::uintmax_t size{};
private:
    friend std::ostream& operator<<(std::ostream& os, HumanReadable hr) {
        int i{};
        double mantissa = static_cast<double>(hr.size);
        for (; mantissa >= 1024.; mantissa /= 1024., ++i) {}
        mantissa = std::ceil(mantissa * 10.) / 10.;
        os << mantissa << "BKMGTPE"[i];
        return i == 0 ? os : os << "B (" << hr.size << ')';
    }
};

class GetFilesList : public SingletonPattern<GetFilesList>
{
    GetFilesList(const GetFilesList& src) = delete;
    GetFilesList& operator=(const GetFilesList& rhs) = delete;

    TimeCash cash;
    bool exploreFolder(const std::string& name);

public:
    static void printIt(const DirListType& stuff);

    GetFilesList() = default;
    virtual ~GetFilesList() = default;
    
    bool getFilesList(const std::string& name, DirListType& out_list);
    bool getFileStatus(const std::filesystem::path& name, FileInfo& f);
};

