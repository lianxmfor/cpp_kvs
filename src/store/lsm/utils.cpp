#include <filesystem>
#include <iostream>

#include "store/lsm/utils.h"

auto file_with_ext(const std::filesystem::path& dir, const string& ext) -> vector<string>
{
    vector<string> files {};
    for (auto& p : std::filesystem::directory_iterator(dir)) {
        if (p.path().extension() == ext) {
            files.push_back(p.path());
        }
    }

    return std::move(files);
}

auto get_random_string(const int len, const std::string& extension) -> string
{
    constexpr static const char alphanum[] = "0123456789"
                                             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                             "abcdefghijklmnopqrstuvwxyz";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, sizeof(alphanum));

    std::string tmp_s;
    tmp_s.reserve(len + extension.size());

    for (int i = 0; i < len; i++) {
        tmp_s += alphanum[distrib(gen) % (sizeof(alphanum) - 1)];
    }

    return tmp_s + "." + extension;
}
