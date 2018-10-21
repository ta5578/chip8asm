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
            val += static_cast<uint16_t>(cl - 'a' + 10);
        } else if (std::isdigit(cl)) {
            val += static_cast<uint16_t>(cl - '0');
        }
    }
    return val;
}

inline std::string from_hex(uint16_t num)
{
    char buf[] = { '0', '0', '0', '0' };
    for (int i = 0; num != 0; i++) {
        const uint16_t d = num % 16;
        if (d >= 10 && d <= 15) {
            buf[i] = static_cast<char>('A' + (d - 10));
        } else {
            buf[i] = static_cast<char>('0' + d);
        }
        num /= 16;
    }

    std::string s = "0x";
    for (int i = 4 - 1; i >= 0; --i) {
        s += buf[i];
    }
    return s;
}

inline bool is_valid_hex_char(char c)
{
    const auto lc = std::tolower(c);
    return std::isdigit(lc) || (static_cast<char>(lc) >= 'a' && static_cast<char>(lc) <= 'f');
}

inline bool is_register(const std::string& s)
{
    if (s.size() != 2) {
        return false;
    }
    return static_cast<char>(std::tolower(s[0])) == 'r' && is_valid_hex_char(s[1]);
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