#pragma once

#include <cstdio>
#include "FileBit.h"
#include <string>
#include <memory>

class FileReader {

private:
    std::shared_ptr<std::FILE> fp;
    std::size_t size_;
    char flags[3];
    std::size_t calculate_size() const;

public:
    FileReader(const char *path, FileBit bit = FileBit::TEXT);

    /* Read up to bytes from the file and return it as a string */
    std::string read(std::size_t bytes);

    std::string read_all();

    /* Return the size of the file in bytes */
    std::size_t size() const;
};