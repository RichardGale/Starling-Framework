// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "UtilsTest.h"
#include "flexunit/framework/Assert.h"

#include "starling/utils/deg2rad.h"
#include "starling/utils/formatString.h"
#include "starling/utils/getNextPowerOfTwo.h"
#include "starling/utils/rad2deg.h"

using namespace flexunit::framework;
using namespace starling::utils;

namespace tests {



        void UtilsTest::                testFormatString()
        {
            Assert::assertEquals("This is a test.", formatString("This is {0} test.", "a"));
            Assert::assertEquals("aba{2}", formatString("{0}{1}{0}{2}", "a", "b"));
            Assert::assertEquals("1{2}21", formatString("{0}{2}{1}{0}", 1, 2));
        }


        void UtilsTest::                testGetNextPowerOfTwo()
        {
            Assert::assertEquals(1,   getNextPowerOfTwo(0));
            Assert::assertEquals(1,   getNextPowerOfTwo(1));
            Assert::assertEquals(2,   getNextPowerOfTwo(2));
            Assert::assertEquals(4,   getNextPowerOfTwo(3));
            Assert::assertEquals(4,   getNextPowerOfTwo(4));
            Assert::assertEquals(8,   getNextPowerOfTwo(6));
            Assert::assertEquals(32,  getNextPowerOfTwo(17));
            Assert::assertEquals(64,  getNextPowerOfTwo(63));
            Assert::assertEquals(256, getNextPowerOfTwo(129));
            Assert::assertEquals(256, getNextPowerOfTwo(255));
            Assert::assertEquals(256, getNextPowerOfTwo(256));
        }


        void UtilsTest::                testRad2Deg()
        {
            Assert::assertEquals(  0.0, rad2deg(0));
            Assert::assertEquals( 90.0, rad2deg(Math::PI() / 2.0));
            Assert::assertEquals(180.0, rad2deg(Math::PI));
            Assert::assertEquals(270.0, rad2deg(Math::PI() / 2.0 * 3.0));
            Assert::assertEquals(360.0, rad2deg(2 * Math::PI));
        }


        void UtilsTest::                testDeg2Rad()
        {
            Assert::assertEquals(0.0, deg2rad(0));
            Assert::assertEquals(Math::PI() / 2.0, deg2rad(90.0));
            Assert::assertEquals(Math::PI, deg2rad(180.0));
            Assert::assertEquals(Math::PI() / 2.0 * 3.0, deg2rad(270.0));
            Assert::assertEquals(2 * Math::PI, deg2rad(360.0));
        }
}

