// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "ColorTest.h"
#include "flexunit/framework/Assert.h"

#include "starling/utils/Color.h"

using namespace flexunit::framework;
using namespace starling::utils;

namespace tests {



        void ColorTest::                testGetElement()
        {
             unsigned int color=0xaabbcc;
            Assert::assertEquals(0xaa, Color::getRed(color));
            Assert::assertEquals(0xbb, Color::getGreen(color));
            Assert::assertEquals(0xcc, Color::getBlue(color));
        }


        void ColorTest::                testRgb()
        {
             unsigned int color=Color::rgb(0xaa,0xbb,0xcc);
            Assert::assertEquals(0xaabbcc, color);
        }


        void ColorTest::                testArgb()
        {
             unsigned int color=Color::argb(0xaa,0xbb,0xcc,0xdd);
            Assert::assertEquals(0xaabbccdd, color);
        }
}

