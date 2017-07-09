#pragma once

#include "utils.h"

SUITE(UTILS_TESTS)
{
    TEST(ToHexEmptyString)
    {
        auto val = to_hex("");
        CHECK_EQUAL(0, val);
    }

    TEST(ToHex1)
    {
        auto val = to_hex("$EEE");
        CHECK_EQUAL(0xEEE, val);
    }

    TEST(ToHexCapital)
    {
        auto val = to_hex("F");
        CHECK_EQUAL(0x000F, val);
    }

    TEST(ToHexLower)
    {
        auto val = to_hex("a");
        CHECK_EQUAL(0x000A, val);
    }

    TEST(ToHexLeadingRegisterCapital)
    {
        auto val = to_hex("RF");
        CHECK_EQUAL(0x000F, val);
    }

    TEST(ToHexLeadingRegisterLower)
    {
        auto val = to_hex("rF");
        CHECK_EQUAL(0x000F, val);
    }

    TEST(TestIsRegisterCapital)
    {
        CHECK(is_register("R2"));
    }

    TEST(TestIsRegisterLower)
    {
        CHECK(is_register("r2"));
    }

    TEST(TestIsRegisterTooLong)
    {
        CHECK(!is_register("r23"));
    }

    TEST(TestIsRegisterMissingPrefix)
    {
        CHECK(!is_register("23"));
    }

    TEST(OneOfEmpty)
    {
        CHECK(!one_of<std::string>("ABC", {""}));
    }

    TEST(OneOfShouldPass)
    {
        CHECK(one_of<std::string>("ABC", {"LOL", "NOWAI", "ABC", "YAH"}));
    }

    TEST(IsValidHexCharDigits)
    {
        for (char c = '1'; c <= '9'; ++c) {
            REQUIRE CHECK(is_valid_hex_char(c));
        }
    }

    TEST(IsValidHexCharCapitalLetters)
    {
        for (char c = 'A'; c <= 'F'; ++c) {
            REQUIRE CHECK(is_valid_hex_char(c));
        }
    }

    TEST(IsValidHexCharLowerLetters)
    {
        for (char c = 'a'; c <= 'a'; ++c) {
            REQUIRE CHECK(is_valid_hex_char(c));
        }
    }

    TEST(FromHexMax)
    {
        auto val = from_hex(65535);
        CHECK_EQUAL("0xFFFF", val);
    }

    TEST(FromHexMin)
    {
        auto val = from_hex(0);
        CHECK_EQUAL("0x0000", val);
    }

    TEST(FromHexMid)
    {
        auto val = from_hex(31);
        CHECK_EQUAL("0x1F", val);
    }
}