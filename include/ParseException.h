#pragma once

#include <exception>
#include <string>

class ParseException : public std::exception {

private:
    const std::string& msg;

public:
    ParseException(const std::string& msg = "") : msg(msg) {}

    const char* what() const noexcept { return msg.c_str(); }
};