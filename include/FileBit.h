#pragma once

enum class FileBit {
    TEXT = 1 << 0, /* Text mode */
    BINARY = 1 << 1, /* Binary mode */
    TRUNCATE = 1 << 2, /* Truncate the file if writing to it */
    APPEND = 1 << 3 /* Append to the file if writing to it */
};

/* Overload the operator for bit mask extraction */
inline bool operator&(FileBit a, FileBit b)
{
    return (static_cast<int>(a) & static_cast<int>(b)) != 0;
}