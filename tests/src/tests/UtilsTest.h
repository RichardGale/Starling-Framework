#if !defined(__TESTS_SRC_TESTS_UTILSTEST_AS)
#define __TESTS_SRC_TESTS_UTILSTEST_AS
#if defined(__cplusplus)
// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "flex11.6.h"
namespace flexunit
{
    namespace framework
    {
        class Assert;
    }
}

namespace starling
{
    namespace utils
    {
        class deg2rad;
    }
}
namespace starling
{
    namespace utils
    {
        class formatString;
    }
}
namespace starling
{
    namespace utils
    {
        class getNextPowerOfTwo;
    }
}
namespace starling
{
    namespace utils
    {
        class rad2deg;
    }
}

using namespace flexunit::framework;
using namespace starling::utils;

namespace tests
{
    class UtilsTest
    {

    public:
        void
        ////[Test]      testFormatString();


    public:
        void
        ////[Test]      testGetNextPowerOfTwo();


    public:
        void
        ////[Test]      testRad2Deg();


    public:
        void
        ////[Test]      testDeg2Rad();
    };
}

#endif // __TESTS_SRC_TESTS_UTILSTEST_AS
#endif // __cplusplus

