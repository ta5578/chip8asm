#pragma once

#include <cstdio>
#include <memory>

class FileWriter {
private:
    std::shared_ptr<std::FILE> fp;
    char flags[3];

public:
    FileWriter(const char *path, FileBit bit = FileBit::TEXT | FileBit::TRUNCATE)
    {
        flags[0] = 'w';
        if (bit & FileBit::BINARY) {
            flags[1] = 'b';
        } else {
            flags[1] = '\0';
        }
        flags[2] = '\0';

        FILE *file = std::fopen(path, flags);
        if (!file) {
            throw std::invalid_argument("The supplied path couldn't be opened for writing!");
        }
        fp = std::shared_ptr<std::FILE>(file, std::fclose);
    }

    template <class T>
    size_t write(const T& obj)
    {
        return std::fwrite(&obj, sizeof(obj), 1, fp.get());
    }
};
