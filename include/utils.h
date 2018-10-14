#pragma once

#include <string>
#include <initializer_list>
#include <cstdio>
#include <cctype>

inline uint16_t endi(uint16_t num)
{
    #if (__BYTE_ORDER == __LITTLE_ENDIAN)
        return (num & 0x00FF) << 8 | (num & 0xFF00) >> 8;
    #elif (__BYTE_ORDER == __BIG_ENDIAN)
        return num;
    #else
        #error "Couldn't determine endianess!"
    #endif
}

/* Given ^\$[0-9]+ string and return a hexadecimal representation */
inline uint16_t to_hex(const std::string& s)
{
    if (s.empty()) {
        return 0;
    }

    uint16_t val = 0x0000;
    for (const char c : s) {
        val *= 16;
        auto cl = std::tolower(c);
        if (cl >= 'a' && cl <= 'f') {
            val += (cl - 'a' + 10);
        } else if (std::isdigit(cl)) {
            val += (cl - '0');
        }
    }
    return val;
}

inline std::string from_hex(uint16_t num)
{
    if (num == 0) {
        return "0x0000";
    }

    char buf[sizeof(num)];

    int i = 0;
    while (num) {
        uint16_t d = num % 16;
        if (d >= 10 && d <= 15) {
            buf[i] = ('A' + (d - 10));
        } else {
            buf[i] = ('0' + d);
        }
        num /= 16;
        ++i;
    }

    std::string s = "0x";
    for (i = i - 1; i >= 0; --i) {
        s += buf[i];
    }
    return s;
}

inline bool is_valid_hex_char(char c)
{
    const char lc = std::tolower(c);
    return std::isdigit(lc) || (lc >= 'a' && lc <= 'f');
}

inline bool is_register(const std::string& s)
{
    if (s.size() != 2) {
        return false;
    }
    return std::tolower(s[0]) == 'r' && is_valid_hex_char(s[1]);
}

template <class T>
inline bool one_of(const T& t, std::initializer_list<T> list)
{
    for (const auto& elem : list) {
        if (t == elem) {
            return true;
        }
    }
    return false;
}