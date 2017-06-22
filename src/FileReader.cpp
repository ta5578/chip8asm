#include "FileReader.h"
#include <stdexcept>

FileReader::FileReader(const char *path, FileBit bit)
{
    flags[0] = 'r';
    if (bit & FileBit::BINARY) {
        flags[1] = 'b';
    } else {
        flags[1] = '\0';
    }
    flags[2] = '\0';

    FILE *file = fopen(path, flags);
    if (!file) {
        throw std::invalid_argument("The supplied path couldn't be opened in for reading!");
    }
    fp = std::shared_ptr<std::FILE>(file, std::fclose);
    size_ = calculate_size();
}

std::string FileReader::read(size_t bytes)
{
    std::string s;
    s.reserve(bytes);
    fread(&s[0], sizeof(s[0]), bytes, fp.get());
    return s;
}

std::string FileReader::read_all()
{
    return read(size());
}

size_t FileReader::size() const
{
    return size_;
}

size_t FileReader::calculate_size() const
{
    /* Get where we are in the file first */
    size_t pos = ftell(fp.get());

    /* Go to the beginning */
    fseek(fp.get(), 0, SEEK_SET);

    /* Now go to the end */
    fseek(fp.get(), 0, SEEK_END);

    /* Get the size of the file now (end - beginning) */
    size_t fsize = ftell(fp.get());

    /* Rewind to where we were */
    fseek(fp.get(), pos, SEEK_SET);

    return fsize;
}