#pragma once

#include "opcodes.h"

SUITE(OPCODES_TESTS)
{
    TEST(TestSYS)
    {
        auto val = fxnSYS({});
        CHECK_EQUAL(0x0, val);
    }

    TEST(TestCLR)
    {
        auto val = fxnCLR({});
        CHECK_EQUAL(0x00E0, val);
    }

    TEST(TestRET)
    {
        auto val = fxnRET({});
        CHECK_EQUAL(0x00EE, val);
    }

    TEST(TestJMP)
    {
        auto val = fxnJMP({"ABC"});
        CHECK_EQUAL(0x1ABC, val);
    }

    TEST(TestCALL)
    {
        auto val = fxnCALL({"ABC"});
        CHECK_EQUAL(0x2ABC, val);
    }

    TEST(TestSKE)
    {
        auto val = fxnSKE({"RF", "AB"});
        CHECK_EQUAL(0x3FAB, val);
    }

    TEST(TestSKNE)
    {
        auto val = fxnSKNE({"RF", "AB"});
        CHECK_EQUAL(0x4FAB, val);
    }

    TEST(TestSKRE)
    {
        auto val = fxnSKRE({"RA", "RB"});
        CHECK_EQUAL(0x5AB0, val);
    }

    TEST(TestLOAD)
    {
        auto val = fxnLOAD({"R1", "$AB"});
        CHECK_EQUAL(0x61AB, val);
    }

    TEST(TestADD)
    {
        auto val = fxnADD({"R1", "$AB"});
        CHECK_EQUAL(0x71AB, val);
    }

    TEST(TestASN)
    {
        auto val = fxnASN({"RA", "RB"});
        CHECK_EQUAL(0x8AB0, val);
    }

    TEST(TestOR)
    {
        auto val = fxnOR({"RA", "RB"});
        CHECK_EQUAL(0x8AB1, val);
    }

    TEST(TestAND)
    {
        auto val = fxnAND({"RA", "RB"});
        CHECK_EQUAL(0x8AB2, val);
    }

    TEST(TestXOR)
    {
        auto val = fxnXOR({"RA", "RB"});
        CHECK_EQUAL(0x8AB3, val);
    }

    TEST(TestRADD)
    {
        auto val = fxnRADD({"RA", "RB"});
        CHECK_EQUAL(0x8AB4, val);
    }

    TEST(TestSUB)
    {
        auto val = fxnSUB({"RA", "RB"});
        CHECK_EQUAL(0x8AB5, val);
    }

    TEST(TestSHR)
    {
        auto val = fxnSHR({"R2"});
        CHECK_EQUAL(0x8206, val);
    }

    TEST(TestRSUB)
    {
        auto val = fxnRSUB({"RA", "RB"});
        CHECK_EQUAL(0x8AB7, val);
    }

    TEST(TestSHL)
    {
        auto val = fxnSHL({"R2"});
        CHECK_EQUAL(0x820E, val);
    }

    TEST(TestSKRNE)
    {
        auto val = fxnSKRNE({"RA", "RB"});
        CHECK_EQUAL(0x9AB0, val);
    }

    TEST(TestILOAD)
    {
        auto val = fxnILOAD({"$ABC"});
        CHECK_EQUAL(0xAABC, val);
    }

    TEST(TestZJMP)
    {
        auto val = fxnZJMP({"$ABC"});
        CHECK_EQUAL(0xBABC, val);
    }

    TEST(TestRAND)
    {
        auto val = fxnRAND({"R8"});
        CHECK_EQUAL(0xC808, val);
    }

    TEST(TestDRAW)
    {
        auto val = fxnDRAW({"r0", "r1", "$6"});
        CHECK_EQUAL(0xD016, val);
    }

    TEST(TestSKK)
    {
        auto val = fxnSKK({"9"});
        CHECK_EQUAL(0xE99E, val);
    }

    TEST(TestSKNK)
    {
        auto val = fxnSKNK({"9"});
        CHECK_EQUAL(0xE9A1, val);
    }

    TEST(TestDELA)
    {
        auto val = fxnDELA({"r4"});
        CHECK_EQUAL(0xF407, val);
    }

    TEST(TestKEYW)
    {
        auto val = fxnKEYW({"rF"});
        CHECK_EQUAL(0xFF0A, val);
    }

    TEST(TestDELR)
    {
        auto val = fxnDELR({"rF"});
        CHECK_EQUAL(0xFF15, val);
    }

    TEST(TestSNDR)
    {
        auto val = fxnSNDR({"rF"});
        CHECK_EQUAL(0xFF18, val);
    }

    TEST(TestIADD)
    {
        auto val = fxnIADD({"rF"});
        CHECK_EQUAL(0xFF1E, val);
    }

    TEST(TestSILS)
    {
        auto val = fxnSILS({"rF"});
        CHECK_EQUAL(0xFF29, val);
    }

    TEST(TestBCD)
    {
        auto val = fxnBCD({"rF"});
        CHECK_EQUAL(0xFF33, val);
    }

    TEST(TestDUMP)
    {
        auto val = fxnDUMP({"rF"});
        CHECK_EQUAL(0xFF55, val);
    }

    TEST(TestIDUMP)
    {
        auto val = fxnIDUMP({"rF"});
        CHECK_EQUAL(0xFF65, val);
    }

    TEST(TestLB)
    {
        auto val = fxnLB({"$ABC"});
        CHECK_EQUAL(0xABC, val);
    }
}