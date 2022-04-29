#pragma once

#include <filesystem>
#include <fstream>
#include <random>
#include <string>
#include <vector>

using std::string;
using std::vector;

auto file_with_ext(const std::filesystem::path& dir, const string& ext) -> vector<string>;

auto get_random_string(const int len, const std::string& extension) -> string;

class TempFile {
private:
    std::filesystem::path path;

public:
    explicit TempFile(const std::string& extension)
    {
        auto filename = get_random_string(10, extension);

        path = std::filesystem::temp_directory_path() / filename;

        // create tempfile
        std::fstream file(path, std::ios::binary | std::ios::app);
        file.close();
    }

    ~TempFile()
    {
        std::filesystem::remove(path);
    }

    auto get_filepath() const -> std::filesystem::path
    {
        return path;
    }
};
