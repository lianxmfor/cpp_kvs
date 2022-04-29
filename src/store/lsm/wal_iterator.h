#pragma once

#include <fstream>
#include <string>

#include "store/lsm/wal.h"

using std::string;

class WALEntryScan {
private:
    string filepath;
    std::ifstream file;

public:
    explicit WALEntryScan(const string& filepath);
    ~WALEntryScan();

public:
    bool has_next();

    WALEntry next();
};
